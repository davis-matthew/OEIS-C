/*
 * A049004  First letter of English names for months of year, mapping A -> 1,
 *          B -> 2 etc.
 * https://oeis.org/A049004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, word, dumb
 * Formula:   a(n) = value of the first letter of the English month name for
 *            (n‑1) mod 12, with A=1, B=2, …, Z=26
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const char *monthNames[12] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };
    idx_t m = (n - 1) % 12;            /* zero‑based month index */
    char c = monthNames[m][0];         /* first letter of month name  */
    return (seq_t)(c - 'A' + 1);       /* map A→1 … Z→26              */
}
