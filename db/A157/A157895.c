/*
 * A157895  Coefficients of polynomials of a prime like factor set :
 *          p(x,n)=Sum[x^i, {i, 0, (Prime[n] - 1)/2}]; q(n,n)=Sum[(-1)^i*x^i,
 *          {i, 0, (Prime[n] - 1)/2}]; t(x,n)=If[n == 0, 1, If[n == 1, x + 1,
 *          (x + 1)*p[x, n]*q[x, n]]].
 * https://oeis.org/A157895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, uned
 * Formula:   not stated
 * Range:     i8 n <= 93, i16 n <= 93, i32 n <= 93, i64 n <= 93, i128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(n) is the coefficient of x^m in
 *           t(x,k) = (x+1)*p(x,k)*q(x,k),
 *           where k and m are chosen so that the flattened index
 *           n corresponds to the coefficient of row k (starting with
 *           k=0) and column m.
 *
 *   p(x,k) = ∑_{i=0}^{(Prime[k]−1)/2} x^i
 *   q(x,k) = ∑_{i=0}^{(Prime[k]−1)/2} (−1)^i x^i
 *
 *   The sequence is flattened row‑by‑row: a(0) = 1 (row 0),
 *   a(1) = 1 (row 1, first coefficient), …
 */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Prime list (first element is 2). */
static uint32_t *primes      = NULL; /* array of primes */
static size_t   primes_sz    = 0;     /* number of primes stored */
static size_t   primes_cap   = 0;     /* allocated capacity */

/* Ensure that at least 'needed' primes are stored. */
static void ensure_primes_until(size_t needed)
{
    if (primes_sz >= needed)
        return;

    size_t new_cap = primes_cap ? primes_cap : 16;
    while (new_cap < needed)
        new_cap <<= 1;

    uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
    if (!tmp) abort();                 /* allocation failed */
    primes     = tmp;
    primes_cap = new_cap;

    uint32_t cand = (primes_sz == 0) ? 2u : primes[primes_sz - 1] + 1u;
    while (primes_sz < needed) {
        int is_prime = 1;
        for (size_t i = 0; i < primes_sz; ++i) {
            uint32_t p = primes[i];
            uint64_t sq = (uint64_t)p * p;
            if (sq > cand) break;
            if (cand % p == 0) { is_prime = 0; break; }
        }
        if (is_prime) primes[primes_sz++] = cand;
        ++cand;
    }
}

/* Helper: compute c_m for given m and d = (p−1)/2. */
static int c_of(uint32_t m, uint32_t d)
{
    if (m > 2u * d) return 0;
    uint32_t a = (m > d) ? (m - d) : 0u;
    uint32_t b = (m < d) ? m : d;
    uint32_t L = b - a + 1u;
    if ((L & 1u) == 0u) return 0;            /* even length → sum 0 */
    return ((a & 1u) == 0u) ? 1 : -1;        /* odd length → ±1 */
}

/* Return coefficient of x^m in row for prime p. */
static int coeff_from_prime(uint32_t p, uint32_t m)
{
    if (p == 2u)                  /* row for prime 2 is simply x+1 */
        return 1;

    uint32_t d  = (p - 1u) / 2u;
    int cm  = c_of(m, d);
    int cm1 = (m == 0u) ? 0 : c_of(m - 1u, d);
    return cm + cm1;               /* possible values: -1, 0, +1 */
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;     /* first row: constant 1 */

    /* Flattened index after the first row. */
    idx_t idx = n - 1;

    /* Row 1 corresponds to prime 2. */
    size_t row = 1;                 /* number of primes currently considered */

    /* Find the row that contains the index. */
    while (1) {
        ensure_primes_until(row);   /* make sure the required prime is available */

        uint32_t p = primes[row - 1];          /* current prime */

        /* Row length: 2 for prime 2, otherwise p + 1. */
        idx_t row_len = (p == 2u) ? 2 : (idx_t)p + 1;

        if (idx < row_len)
            break;                         /* found the correct row */

        idx -= row_len;                    /* skip to next row */
        ++row;
    }

    uint32_t p      = primes[row - 1];
    uint32_t offset = (uint32_t)idx;
    int coeff = coeff_from_prime(p, offset);
    return (seq_t)coeff;
}
