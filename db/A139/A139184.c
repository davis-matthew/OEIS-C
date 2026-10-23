/*
 * A139184  a(n) = (n! - 9)/9.
 * https://oeis.org/A139184
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n! - 9)/9
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The OEIS sequence is defined for n ≥ 6.
       For values below the offset we return 0. */
    if (n < 6)
        return 0;

    /* When seq_t is an unsigned integer type (≤ 64 bits) we
       perform the calculation in an unsigned 128‑bit integer to
       avoid overflow in the intermediate factorial.  This is
       sufficient for all n for which the result still fits in
       seq_t (i.e. up to n = 21).  For larger n the value overflows,
       but the value is not required to be correct beyond its
       overflow limit. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 f = 1;
        for (idx_t i = 2; i <= n; ++i)
            f *= (unsigned __int128)i;
        return (seq_t)((f - 9) / 9);
    }

    /* For arbitrary‑precision seq_t we can safely compute the
       full factorial and then apply the division.  The helper
       multiplication and subtraction use only the operators
       allowed by the specification. */
    seq_t f = 1;
    for (idx_t i = 2; i <= n; ++i)
        f = f * (seq_t)i;
    return (f - 9) / 9;
}
