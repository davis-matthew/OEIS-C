/*
 * A080993  Point values of letters and blank in Scrabble (English version).
 * https://oeis.org/A080993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 27, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) gives the Scrabble point value of the nth letter or blank, 
   repeating every 27 terms: the 26 letters followed by the blank (value 0). */
seq_t a(idx_t n)
{
    static const seq_t vals[27] = {
        1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10,
        1, 1, 1, 1, 4, 4, 8, 4, 10, 0
    };
    idx_t i = (n - 1) % 27;
    return vals[i];
}
