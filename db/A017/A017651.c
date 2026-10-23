/*
 * A017651  a(n) = (12*n + 10)^11.
 * https://oeis.org/A017651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 10)^11
 * Range:     u8 none, u16 none, u32 none, u64 n <= 3, u128 n <= 264,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base value: (12 * n + 10) */
    seq_t base = ((seq_t)12 * n) + ((seq_t)10);

    /* Compute base^11 by repeated multiplication */
    seq_t result = (seq_t)1;
    for (idx_t i = (idx_t)0; i < (idx_t)11; ++i) {
        result *= base;
    }
    return result;
}
