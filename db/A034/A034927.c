/*
 * A034927  Number of letters in "What is the next number in this sequence?".
 * https://oeis.org/A034927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dumb, fini, full, nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: returns the number of alphabetic letters in the nth word of
   the sentence "What is the next number in this sequence?".
   Words are separated by non‑letters.  If n exceeds the number of words,
   the function returns 0. */
seq_t a(idx_t n)
{
    static const char *sentence = "What is the next number in this sequence?";
    idx_t i = 0;
    seq_t cur_len = 0;
    idx_t cur_word = 1; /* words are indexed starting at 1 */

    while (sentence[i] != '\0') {
        unsigned char c = (unsigned char)sentence[i];
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
            cur_len++;
        } else {
            if (cur_len > 0) {
                if (cur_word == n) return cur_len;
                cur_word++;
                cur_len = 0;
            }
        }
        i++;
    }
    /* Handle last word if string ends without a separator */
    if (cur_len > 0 && cur_word == n) return cur_len;

    return 0;
}
