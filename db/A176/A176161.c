/*
 * A176161  Triangle read by rows: T(n,k) = (1 + Eulerian(n+1, k))^n.
 * https://oeis.org/A176161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, less
 * Formula:   a(n) = (1 + Eulerian(r+1,k))^r where r =
 *            floor((sqrt(8*n+1)-1)/2), k = n - r*(r+1)/2. The triangle T(n,k)
 *            = (1 + Eulerian(n+1,k))^n is read row‑by‑row and flattened into
 *            the sequence a(n)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 23, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t used in function prototypes */
#include <stdint.h>   /* for uintptr_t, if needed */

/* Helper to compute the binomial coefficient C(m, j) as a seq_t value. */
static seq_t binom_idx(idx_t m, idx_t j)
{
    seq_t result = (seq_t)1;
    for (idx_t x = 1; x <= j; ++x) {
        result = result * (seq_t)(m - j + x);
        result = result / (seq_t)x;          /* exact division */
    }
    return result;
}

/* Helper to compute the Eulerian number A(n,k) for 0<=k<n. */
static seq_t eulerian_idx(idx_t n, idx_t k)
{
    seq_t sum = (seq_t)0;
    for (idx_t j = 0; j <= k; ++j) {
        seq_t comb = binom_idx(n + 1, j);    /* C(n+1, j) */
        idx_t base_idx = (k + 1) - j;        /* (k+1-j) */
        seq_t power = (seq_t)1;
        for (idx_t e = 0; e < n; ++e)
            power *= (seq_t)base_idx;        /* (k+1-j)^n by repeated multiplication */
        seq_t term = comb * power;
        if (j & 1)
            sum -= term;
        else
            sum += term;
    }
    return sum;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while (( (__int128)(r + 1) * (r + 2) / 2 ) <= n)
        ++r;

    idx_t triangular = r * (r + 1) / 2;
    idx_t k = n - triangular;                 /* column index within row r */

    /* Compute (1 + Eulerian(r+1,k))^r. */
    seq_t euler = eulerian_idx(r + 1, k);
    seq_t base = euler + (seq_t)1;

    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < r; ++i)
        result *= base;

    return result;
}
