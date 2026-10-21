/*
 * A095948  Positions of vowels in English alphabet.
 * https://oeis.org/A095948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, word
 * Formula:   a(n) = vowels[(n-1)%5] + 26*((n-1)/5)
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const unsigned char vowel_positions[5] = {1,5,9,15,21};

seq_t a(idx_t n)
{
    idx_t m = n - 1;                     /* shift to 0‑based index */
    idx_t rem = m % 5;                  /* which vowel in the cycle */
    idx_t quot = m / 5;                 /* how many full cycles completed */
    seq_t res = (seq_t)vowel_positions[rem] + (seq_t)26 * (seq_t)quot;
    return res;
}
