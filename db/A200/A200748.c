/*
 * A200748  Smallest number requiring n terms to be expressed as a sum of
 *          factorials.
 * https://oeis.org/A200748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest integer whose factorial base digit sum
 *            equals n. The factorial base representation uses digits d_k (0 ≤
 *            d_k ≤ k) with value Σ d_k · k!; the sum of the digits is the
 *            number of factorial terms needed. This function computes that
 *            minimal integer by greedy selection of the smalle...
 * Range:     u8 n <= 11, u16 n <= 28, u32 n <= 73, u64 n <= 196,
 *            u128 n <= 561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t m   = 0;                       /* result */
    seq_t rem = (seq_t)n;                /* remaining digit sum to allocate */
    seq_t fac = 1;                       /* current factorial value i! */

    for (idx_t i = 1; rem; ++i) {
        seq_t use = rem <= i ? rem : (seq_t)i;  /* use as many as possible up to limit i */
        m += use * fac;
        rem -= use;
        fac *= (seq_t)(i + 1);        /* move to next factorial (i+1)! */
    }

    return m;
}
