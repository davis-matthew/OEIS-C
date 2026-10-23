/*
 * A021529  Decimal expansion of 1/525.
 * https://oeis.org/A021529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A021529: Decimal expansion of 1/525
 * Recurrence (periodic after the first two zeros)
 * a(0) = 0
 * a(1) = 0
 * a(n) = {1,9,0,4,7,6}[ (n-2) mod 6 ]  for n ≥ 2
 */

seq_t a(idx_t n)
{
    /* digits that repeat after the first two zeros */
    static const seq_t pattern[6] = {
        (seq_t)1,
        (seq_t)9,
        (seq_t)0,
        (seq_t)4,
        (seq_t)7,
        (seq_t)6
    };

    if (n < 2U) {
        return (seq_t)0;
    }
    /* idx_t is unsigned, so (n-2) is safe */
    idx_t idx = (n - 2U) % 6U;
    return pattern[idx];
}
