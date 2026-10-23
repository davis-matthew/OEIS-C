/*
 * A130801  Number of states in the United States whose first letter is the
 *          n-th letter of the alphabet.
 * https://oeis.org/A130801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn
 * Formula:   a(n) = number of US states whose first letter is the n-th letter
 *            of the alphabet
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* counts for letters A-Z */
    static const seq_t cnt[26] = {
        4,  /* A */
        0,  /* B */
        3,  /* C */
        1,  /* D */
        0,  /* E */
        1,  /* F */
        1,  /* G */
        1,  /* H */
        4,  /* I */
        0,  /* J */
        2,  /* K */
        1,  /* L */
       8,  /* M */
       8,  /* N */
       3,  /* O */
       1,  /* P */
       0,  /* Q */
       1,  /* R */
       2,  /* S */
       2,  /* T */
       1,  /* U */
       2,  /* V */
       4,  /* W */
       0,  /* X */
       0,  /* Y */
       0   /* Z */
    };

    if (n == 0) {
        return 0;
    } else if (n <= 26) {
        return cnt[n - 1];
    } else {
        return 0;
    }
}
