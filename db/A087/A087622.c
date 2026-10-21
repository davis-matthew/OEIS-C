/*
 * A087622  Order of letters on Dvorak typewriter (or computer) keyboard.
 * https://oeis.org/A087622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = position of the nth letter of the standard alphabet when
 *            read in the order of the Dvorak keyboard layout
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Dvorak keyboard letter order (left to right, top to bottom).  */
    static const unsigned char dvorak[26] = {
        /* Top row */
        'P','Y','F','G','C','R','L',
        /* Home row */
        'A','O','E','U','I','D','H','T','N','S',
        /* Bottom row */
        'Q','J','K','X','B','M','W','V','Z'
    };

    if (n == 0) return 0;            /* sequence offset is 1 */
    if (n > 26) return 0;            /* undefined beyond the 26 letters */

    unsigned char ch = dvorak[n - 1]; /* get the nth letter in Dvorak order */
    return (seq_t)(ch - 'A' + 1);     /* convert to 1‑based alphabet index */
}
