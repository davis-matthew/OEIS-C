/*
 * A163649  Triangle interpolating between (-1)^n (A033999) and A056040(n),
 *          read by rows.
 * https://oeis.org/A163649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 25, i16 n <= 72, i32 n <= 243, i64 n <= 887,
 *            i128 n <= 1274, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A163649 – Triangle T(n,k) read by rows, where
    T(n,k) = (-1)^(n-k) * floor(k/2)!^(-2) * n!/(n-k)!
    a(n) is the element obtained by flattening the triangle rowwise
    (row 0: 1, row 1: -1, 1, row 2: 1, -2, 2, …).   */

/*  The implementation uses prime‐factorisation of the ratio
    n!/(n-k)! / (floor(k/2)!)^2.  No large intermediate values
    are ever formed, so the function works correctly for any
    seq_t type that can represent the final result.   */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* --- Prime cache -------------------------------------------------- */
static uint32_t *prime_list = NULL; /* dynamic array of primes    */
static size_t   prime_cnt   = 0;    /* number of primes stored    */
static idx_t    prime_max   = 1;    /* highest number checked so far */

static void ensure_primes(idx_t limit)
{
    if (limit <= prime_max) return;              /* already enough */

    /* Allocate a bit more space than needed to avoid too many reallocs */
    uint32_t *tmp = (uint32_t *)realloc(prime_list,
                                        (size_t)(limit + 10) * sizeof(uint32_t));
    if (!tmp) return;                            /* allocation failure – unlikely on tests */
    prime_list = tmp;

    for (idx_t cand = prime_max + 1; cand <= limit; ++cand) {
        int is_prime = 1;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = prime_list[i];
            if ((idx_t)p * (idx_t)p > cand) break;
            if (cand % p == 0) { is_prime = 0; break; }
        }
        if (is_prime) prime_list[prime_cnt++] = (uint32_t)cand;
    }
    prime_max = limit;
}

/* --- Sequence --------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Find row r and column k from the flattened index n.  */
    idx_t r = 0;
    /* (r+1)(r+2)/2 gives the first index of row r+1 */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    idx_t prev = r * (r + 1) / 2;          /* first index of row r   */
    idx_t k    = n - prev;                 /* column within the row */

    /* Need primes up to r for the exponents. */
    ensure_primes(r);

    seq_t result = 1;

    /* Compute exponent of each prime in the expression
       e_p = exp(n!/(n-k)!) - 2*exp(floor(k/2)!)   */
    idx_t half = k >> 1;   /* floor(k/2) */

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint32_t p = prime_list[pi];
        if (p > r) break;

        /* exponent of p in n!   */
        idx_t exp_n = 0;
        for (idx_t t = p; t <= r; t *= p) exp_n += r / t;

        /* exponent of p in (n-k)! */
        idx_t exp_rk = 0;
        idx_t rk = r - k;
        for (idx_t t = p; t <= rk; t *= p) exp_rk += rk / t;

        /* exponent of p in floor(k/2)! */
        idx_t exp_d = 0;
        for (idx_t t = p; t <= half; t *= p) exp_d += half / t;

        idx_t e = exp_n - exp_rk - 2 * exp_d;
        if (e == 0) continue;

        /* multiply result by p^e */
        for (idx_t i = 0; i < e; ++i) result *= (seq_t)p;
    }

    /* Apply the sign factor (-1)^(n-k).  */
    if ((r - k) & 1ULL) result = -result;

    return result;
}
