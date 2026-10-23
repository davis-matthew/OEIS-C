/*
 * A008777  Number of dots and dashes used when representing n-th letter in
 *          Morse code.
 * https://oeis.org/A008777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, word, full
 * Formula:   a(n) = floor(log_3(A059852(n))) + 1
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Since A059852(n) is the base‑3 number formed by the Morse code for the n‑th letter
   (dot = 1, dash = 2), the number of base‑3 digits equals the length of that Morse
   code.  The Morse code lengths for the 26 letters (A‑Z) are fixed and repeat
   periodically.  We therefore return the precomputed length for
   n mod 26. */
static const seq_t morse_len[26] = {
    2, 4, 4, 3, 1, 4, 3, 4, 2, 4, 3, 4,
    2, 2, 3, 4, 4, 3, 3, 1, 3, 4, 3, 4,
    4, 4
};

seq_t a(idx_t n)
{
    /* Map n to the 0‑based index of the letter, with wrap‑around every 26. */
    idx_t i = (n - 1) % 26;
    return morse_len[i];
}
