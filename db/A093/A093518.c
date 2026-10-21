/*
 * A093518  Number of ways of representing n as the sum of two (not
 *          necessarily distinct) generalized pentagonal numbers.
 * https://oeis.org/A093518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of ways to represent n as the sum of two generalized pentagonal numbers.
   Count unordered pairs {p,q} with p <= q, both p and q in GP = {k(3k-1)/2 | k ∈ ℤ}. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* generate all generalized pentagonal numbers <= n */
    static seq_t gp_vals[256];  /* enough for n up to several millions */
    idx_t gp_cnt = 0;

    /* positive k (including 0) */
    for (idx_t k = 0;; ++k) {
        seq_t val = (seq_t)k * ((seq_t)3 * k - 1) / 2;
        if (val > n)
            break;
        gp_vals[gp_cnt++] = val;
    }
    /* negative k (k = -m, m >= 1) gives m(3m+1)/2 */
    for (idx_t m = 1;; ++m) {
        seq_t val = (seq_t)m * ((seq_t)3 * m + 1) / 2;
        if (val > n)
            break;
        gp_vals[gp_cnt++] = val;
    }

    /* sort gp_vals (insertion sort, gp_cnt <= ~260 for n <= 10000) */
    for (idx_t i = 1; i < gp_cnt; ++i) {
        seq_t tmp = gp_vals[i];
        idx_t j = i;
        while (j > 0 && gp_vals[j - 1] > tmp) {
            gp_vals[j] = gp_vals[j - 1];
            --j;
        }
        gp_vals[j] = tmp;
    }

    /* count unordered pairs p <= q with p + q == n */
    seq_t count = 0;
    idx_t left = 0, right = gp_cnt ? gp_cnt - 1 : 0;
    while (left <= right) {
        seq_t sum = gp_vals[left] + gp_vals[right];
        if (sum == n) {
            ++count;
            if (left == right)   /* pair (x,x) */
                break;
            ++left;
            if (right == 0)      /* avoid underflow for unsigned types */
                break;
            --right;
        } else if (sum < n) {
            ++left;
        } else {  /* sum > n */
            if (right == 0)
                break;
            --right;
        }
    }
    return count;
}
