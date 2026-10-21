/*
 * A069195  Order of first letter of English name of consecutive days of the
 *          week starting from a Monday.
 * https://oeis.org/A069195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word, easy, less
 * Formula:   a(n) = position of the first letter of the weekday name,
 *            starting with Monday (M=13)
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const char *days[7] = {
        "Monday",    "Tuesday",   "Wednesday",
        "Thursday",  "Friday",    "Saturday",
        "Sunday"
    };
    unsigned idx = (unsigned)(n % 7U);
    char c = days[idx][0];
    return (seq_t)(c - 'A' + 1);
}
