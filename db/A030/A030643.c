/*
 * A030643  Map 1 to A, 2 to B, ... 26 to Z and 0 to space and the sequence
 *          reads: "NUMBER SERIES".
 * https://oeis.org/A030643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, word
 * Formula:   a(n) equals the alphabet position of the n‑th character of
 *            "NUMBER SERIES" (A=1…Z=26, space=0). For n>13 the value is 0
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const char phrase[] = "NUMBER SERIES";
    /* length of the phrase */
    const idx_t len = sizeof(phrase) - 1;   /* 13 */
    if (n < 1 || n > len) return 0;

    char c = phrase[n - 1];
    if (c == ' ') return 0;
    return (seq_t)(c - 'A' + 1);
}
