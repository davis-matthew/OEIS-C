/*
 * A021852  Decimal expansion of 1/848.
 * https://oeis.org/A021852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = the n-th digit after the decimal point in the decimal
 *            expansion of 1/848
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 1/848 = 0.001179245283018867924528301886792… */
    idx_t remainder = 1;           /* start remainder 1 */
    seq_t digit = 0;

    /* Perform the long‑division step n+1 times to reach digit a(n). */
    for (idx_t i = 0; i <= n; ++i) {
        remainder *= 10;
        digit = remainder / 848;   /* digit at this position */
        remainder %= 848;          /* new remainder for next step */
    }

    return digit;
}
