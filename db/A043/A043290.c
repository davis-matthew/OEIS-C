/*
 * A043290  Maximal run length in base 16 representation of n.
 * https://oeis.org/A043290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = maximal run length of equal hex digits in the base‑16
 *            representation of n
 * Range:     u8 n <= 1645, u16 n <= 9970, u32 n <= 9970, u64 n <= 9970,
 *            u128 n <= 9970, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the longest consecutive sequence of identical hexadecimal digits in n.
   n is given as an idx_t (unsigned 64‑bit integer).  The result fits in the
   sequence type seq_t and is returned as that type. */
seq_t a(idx_t n)
{
    /* Work on a copy of n in the sequence type */
    seq_t value = (seq_t)n;
    unsigned int max_run = 0;   /* maximum run observed so far */
    unsigned int cur_run = 0;   /* current run length */
    int prev = -1;              /* previous digit (-1 means none yet) */

    while (value > 0) {
        /* Extract the least significant hex digit */
        unsigned int digit = (unsigned int)(value % (seq_t)16);
        value /= (seq_t)16;

        if (prev >= 0 && digit == (unsigned int)prev) {
            /* Continue current run */
            cur_run++;
        } else {
            /* Start a new run */
            cur_run = 1;
            prev = (int)digit;
        }

        if (cur_run > max_run)
            max_run = cur_run;
    }

    return (seq_t)max_run;
}
