/*
 * A161603  Odd terms of sequence A161602.
 * https://oeis.org/A161603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: odd integers whose binary reversal is less than the integer.
   Brute‑force search. */

seq_t a(idx_t n)
{
    idx_t found = 0;
    for (idx_t i = 1;; ++i) {
        /* skip even numbers */
        if ((i & 1) == 0) continue;

        seq_t x = (seq_t)i;
        seq_t rev = 0;
        seq_t y = x;
        /* compute binary reverse of x */
        while (y > 0) {
            rev = (rev << 1) | (y & 1);
            y >>= 1;
        }

        if (rev < x) {
            ++found;
            if (found == n) return x;
        }
    }
    /* unreachable, but keeps the compiler happy */
    return 0;
}
