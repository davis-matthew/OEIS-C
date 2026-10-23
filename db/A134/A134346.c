/*
 * A134346  Triangle read by rows: T(n,k) = (2^(n+1)-1)*binomial(n,k).
 * https://oeis.org/A134346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = (2^(r+1) - 1) * binomial(r, k) where r = floor((sqrt(8*n
 *            + 1) - 1) / 2) and k = n - r*(r + 1)/2 (0 ≤ k ≤ r)
 * Range:     u8 n <= 15, u16 n <= 47, u32 n <= 159, u64 n <= 576,
 *            u128 n <= 2237, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 2^e as a seq_t (generic unsigned type). */
static seq_t pow2(idx_t e)
{
    seq_t result = 1;
    seq_t base   = 2;
    while (e > 0) {
        if (e & 1U) result *= base;
        base *= base;
        e >>= 1U;
    }
    return result;
}

/* Compute C(n, k) multiplicatively (exact division). */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - i + 1);
        res = res / (seq_t)i;
    }
    return res;
}

/* Return the n-th term of OEIS A134346. */
seq_t a(idx_t n)
{
    /* Find r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
    idx_t lo = 0, hi = n, r = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= n) {
            r = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }

    unsigned __int128 used = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)used;

    seq_t p2   = pow2(r + 1);          /* 2^(r+1) */
    seq_t bino = binom(r, k);
    return (p2 - 1) * bino;
}
