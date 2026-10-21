/*
 * A053545  Comparisons needed for Batcher's sorting algorithm applied to 2^n
 *          items.
 * https://oeis.org/A053545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2^(n-2)*(n^2 - n + 4) - 1 for n ≥ 2 a(0) = 0, a(1) = 1
 *            (The recurrence given in the OEIS entry is linear and can be
 *            avoided by this closed form.)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 54, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 2^exp in the arbitrary–precision type seq_t. */
static seq_t pow2_seq_t(idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)2;
    while (exp) {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    /* Use the closed‑form formula for n ≥ 2 */
    idx_t      exp   = n - 2;
    seq_t      pow2  = pow2_seq_t(exp);
    seq_t      nseq  = (seq_t)n;
    seq_t      quad  = nseq * nseq - nseq + (seq_t)4;   /* n^2 - n + 4 */
    seq_t      val   = pow2 * quad;
    val -= (seq_t)1;
    return val;
}
