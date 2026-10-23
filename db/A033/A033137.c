/*
 * A033137  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 1,1,0.
 * https://oeis.org/A033137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(110 * 10^n / 999)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A033137: decimal expansion of the periodic pattern 1,1,0 */
seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t i = 1; i <= n; ++i) {
        /* pattern: 1,1,0 repeats */
        seq_t digit = (i % 3 == 0) ? 0 : 1;
        result = result * 10 + digit;
    }
    return result;
}
