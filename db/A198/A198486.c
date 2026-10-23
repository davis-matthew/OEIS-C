/*
 * A198486  Numbers with the property that all pairs of consecutive digits
 *          differ by 9.
 * https://oeis.org/A198486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 17, u64 n <= 27, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of numbers with consecutive digits differing by 9.
   a(n) = n          for n <= 9
   a(n) = number with (n‑8) digits alternating 9 and 0 starting with 9
   for n >= 10. */
seq_t a(idx_t n)
{
    if (n <= 9)
        return (seq_t)n;          /* single‑digit numbers 1..9 */

    idx_t digits = n - 8;          /* number of digits for n >= 10 */
    seq_t  res = 0;

    for (idx_t i = 0; i < digits; ++i) {
        seq_t digit = (i % 2 == 0) ? 9 : 0; /* 9,0,9,0,... */
        res = res * 10 + digit;
    }
    return res;
}
