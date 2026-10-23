/*
 * A152775  Numbers with 3n binary digits where every run length is 3, written
 *          in binary.
 * https://oeis.org/A152775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 1000*a(n-1) + a(n-2) - 1000*a(n-3) with a(1)=111,
 *            a(2)=111000, a(3)=111000111. The numbers are, in decimal
 *            notation, formed by concatenating 3‑digit runs of 1’s and 0’s,
 *            starting with 111 and alternating
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t i = 1; i <= n; ++i) {
        res *= 1000;                /* shift decimal digits by 3 places */
        if (i & 1)                   /* odd run: append '111' */
            res += 111;
    }
    return res;
}
