/*
 * A178457  Partial sums of floor(2^n/23).
 * https://oeis.org/A178457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..n} ⌊2^k / 23⌋ The recurrence below updates the
 *            floor value q_k = ⌊2^k/23⌋ using only non‑negative integer
 *            operations: r_{k} = (2 * r_{k-1}) mod 23 (modular part of 2^k)
 *            carry_k = ⌊2 * r_{k-1} / 23⌋ q_k = 2 * q_{k-1} + carry_k The sum
 *            a(n) is accumulated on the fly without caching i...
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for idx_t and seq_t – provided by the build */

/* Public entry point for OEIS sequence A178457 */
seq_t a(idx_t n)
{
    /* Special case for n = 0 */
    if (n == 0)
        return (seq_t)0;

    seq_t sum = (seq_t)0;          /* a(0) = 0, will accumulate the rest   */
    seq_t q   = (seq_t)0;          /* q_0 = ⌊2^0 / 23⌋ = 0                 */
    unsigned char r = 1u;          /* r_0 = 2^0 mod 23 = 1                 */

    for (idx_t k = 1; k <= n; ++k) {
        unsigned int r2   = (unsigned int)r * 2u;   /* 2 * r_{k-1}          */
        unsigned int carry = r2 / 23u;              /* ⌊2 * r_{k-1} / 23⌋   */
        r = (unsigned char)(r2 % 23u);             /* r_k = 2^k mod 23    */

        /* q_k = 2*q_{k-1} + carry;   all non‑negative          */
        q = q * (seq_t)2 + (seq_t)carry;

        sum += q;                                 /* accumulate a_k       */
    }

    return sum;
}
