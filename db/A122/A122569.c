/*
 * A122569  Consider the Thue-Morse sequence (A010059) at each iteration. Read
 *          each reversed string as a binary number and convert it to a
 *          decimal number.
 * https://oeis.org/A122569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{m=1..2^(n-1)} T(n,m)*2^(m-1) * where T(n,m) is the
 *            m‑th bit of the Thue‑Morse word at iteration n. * The
 *            implementation uses the identity * a(n) = \sum_{k=0}^{2^{n-1}-1}
 *            (popcount(k) mod 2 == 0) * 2^k * and builds a(n) iteratively
 *            from a(n-1) with only big‑integer arithmetic
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast exponentiation of 2.  e fits in idx_t (uint64_t). */
static seq_t pow2(idx_t e)
{
    seq_t result = 1;
    seq_t base   = 2;
    while (e)
    {
        if (e & 1)
            result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* a(1) = 1 */
    if (n <= 1)
        return (seq_t)1;

    seq_t val = (seq_t)1;          /* a(1) */

    for (idx_t k = 2; k <= n; ++k)
    {
        /* length of the previous Thue‑Morse word */
        idx_t L = (idx_t)1 << (k - 2);

        /* mask for all lower L bits: (1 << L) - 1 */
        seq_t mask = pow2(L) - (seq_t)1;

        /* bits that are 0 in val, limited to L bits */
        seq_t comp = (~val) & mask;

        /* place these bits in the higher half: shift by L */
        seq_t shifted = comp * pow2(L);

        /* combine lower and upper halves */
        val |= shifted;
    }
    return val;
}
