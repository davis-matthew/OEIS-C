/*
 * A103131  The product of the residues in [1,n] relatively prime to n, taken
 *          modulo n.
 * https://oeis.org/A103131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ∏{k | 1 ≤ k < n, gcd(k,n)=1} k (mod n), expressed in the
 *            minimal absolute value representation
 * Range:     i8 n <= 1180, i16 n <= 9816, i32 n <= 9816, i64 n <= 9816,
 *            i128 n <= 9816, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean gcd for unsigned indices. */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1)=0, and n<1 is undefined – return 0.   */
        return (seq_t)0;

    seq_t prod = (seq_t)1;
    seq_t sn   = (seq_t)n;            /* cast once to avoid repeated casts   */

    for (idx_t k = 1; k < n; ++k) {
        if (gcd_idx(k, n) == 1)
            prod = (prod * (seq_t)k) % sn;
    }

    /* Convert to minimal signed representation. */
    seq_t half = sn / (seq_t)2;       /* integer division truncates to floor  */
    if (prod > half)
        prod -= sn;

    return prod;
}
