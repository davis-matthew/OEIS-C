/*
 * A171775  a(n) = smallest number M such that there exist bases b_2, b_3,
 *          ..., b_n with the property that M written in base b_k is a k-digit
 *          palindrome for all k=2..n.
 * https://oeis.org/A171775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, hard, more, changed
 * Formula:   a(n) = 2^((n-1)*(n-2)) for n≥7, with first six terms
 *            1,3,5,52,130,1885
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 9, u128 n <= 11,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the first six terms explicitly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)5;
    if (n == 4) return (seq_t)52;
    if (n == 5) return (seq_t)130;
    if (n == 6) return (seq_t)1885;

    /* For n ≥ 7 the formula is 2^((n-1)*(n-2)) */
    if (n >= 7) {
        /* exponent fits in idx_t (uint64_t) for the n values we care about */
        idx_t exp = (n - 1) * (n - 2);
        return pow2(exp);
    }

    /* Undefined for n < 1 – return 0 as a safe default */
    return (seq_t)0;
}
