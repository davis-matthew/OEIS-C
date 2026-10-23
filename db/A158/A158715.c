/*
 * A158715  Smallest prime with n distinct nonprime decimal digits.
 * https://oeis.org/A158715
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest prime with n distinct nonprime decimal digits.
   n = 2..6: return the known smallest prime.
   For n outside this range, return 0 (no such prime). */
seq_t a(idx_t n)
{
    static const seq_t tbl[7] = {0, 0, 19, 109, 1049, 14869, 104869};
    if (n < 2 || n > 6)
        return 0;
    return tbl[n];
}
