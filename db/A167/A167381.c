/*
 * A167381  The numbers read down the left-center column of an arrangement of
 *          the natural numbers in square blocks.
 * https://oeis.org/A167381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   let k be the smallest integer such that k(k+1) ≥ n (equivalently
 *            k = ceil((√(1+4n) – 1)/2)). Then a(n) = 1 + 2*(k-1)*k*(2k-1)/3 +
 *            (k-1) + (n - k*(k-1) - 1) * (2k) All arithmetic is performed in
 *            seq_t to avoid overflow
 * Range:     u8 n <= 30, u16 n <= 1341, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Find the smallest k such that k*(k+1) >= n.
   Uses a binary search on k. */
static idx_t find_k(idx_t n)
{
    idx_t lo = 1, hi = 1;
    /* first make hi large enough */
    while ((__uint128_t)hi * (hi + 1) < n)
        hi <<= 1;

    /* binary search */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((__uint128_t)mid * (mid + 1) >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; if n==0 return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Determine the block k that contains position n. */
    idx_t k = find_k(n);                /* k >= 1 */
    idx_t prev_cum = (k - 1) * k;       /* cumulative terms before block k */
    idx_t t = n - prev_cum - 1;         /* 0‑based index within the block */

    /* Compute base = 1 + 2*(k-1)*k*(2k-1)/3 */
    seq_t km1 = (seq_t)(k - 1);
    seq_t kk  = (seq_t)k;
    seq_t two_k_minus1 = (seq_t)(2 * k - 1);
    seq_t tmp = km1 * kk * two_k_minus1;          /* (k-1)k(2k-1) */
    seq_t base = (seq_t)1 + ((seq_t)2 * tmp) / (seq_t)3;  /* 2/3 * tmp is integer */

    /* a(n) = base + t * (2k) + (k-1) */
    seq_t two_k = (seq_t)(2 * k);
    seq_t offset = (seq_t)t;
    seq_t result = base + offset * two_k + km1;
    return result;
}
