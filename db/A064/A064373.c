/*
 * A064373  "60-60-842", a popular song by the B52's.
 * https://oeis.org/A064373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = digit of the pattern 6,0,6,0,8,4,2 at position n (period
 *            7)
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic pattern of digits in the song title "60-60-842" */
    static const seq_t pat[7] = {6, 0, 6, 0, 8, 4, 2};
    return pat[n % 7];
}
