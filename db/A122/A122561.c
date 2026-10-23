/*
 * A122561  Position of letters from the sentence: 'aleph is one' in alphabet.
 * https://oeis.org/A122561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the alphabetical position (A=1, B=2, ...) of the nth alphabetic character
 * of the fixed string "aleph is one". For n larger than the number of letters,
 * the function returns 0. */
seq_t a(idx_t n)
{
    static const char *msg = "aleph is one";
    idx_t pos = 0;                 /* count of alphabetic characters processed   */
    const char *p = msg;
    while (*p) {
        char c = *p;
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
            pos++;
            if (pos == n) {
                /* Convert to lowercase; works for ASCII letters */
                char lc = (c | 0x20);
                return (seq_t)(lc - 'a' + 1);
            }
        }
        ++p;
    }
    return 0;
}
