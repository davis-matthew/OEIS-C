/*
 * A225901  Write n in factorial base, then replace each nonzero digit d of
 *          radix k with k-d.
 * https://oeis.org/A225901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum_{k} (k+1 - d_k) * k! , where the factorial-base
 *            digits d_k of n satisfy n = sum_{k} d_k * k! and 0 ≤ d_k ≤ k.
 *            The sum runs over all k with k! ≤ n. a(0) = 0
 * Range:     u8 n <= 119, u16 n <= 5039, u32 n <= 5039, u64 n <= 5039,
 *            u128 n <= 5039, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;

    /* Pre‑compute factorials up to the largest k with k! ≤ n.
       The array size 20 is more than enough for 64‑bit n. */
    seq_t facs[20];
    size_t count = 0;
    seq_t fac = 1;          /* 1! */
    for (idx_t k = 1; fac <= n; ++k) {
        facs[count++] = fac;
        fac = fac * (seq_t)(k + 1);   /* (k+1)! for next iteration */
    }

    seq_t result = 0;
    idx_t rem = n;

    /* Process digits from most significant (largest k) to 1. */
    for (size_t i = count; i > 0; --i) {   /* i == k */
        seq_t f = facs[i - 1];            /* k! */
        idx_t fk = (idx_t)f;              /* for division */
        idx_t d = rem / fk;               /* current factorial digit */
        rem -= d * fk;
        if (d != 0) {
            seq_t nd = (seq_t)(i + 1) - (seq_t)d;   /* new digit */
            result += nd * f;
        }
    }
    return result;
}
