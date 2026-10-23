/*
 * A043365  Duplicate of A023738.
 * https://oeis.org/A043365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) enumerates the numbers n such that for m = 0,1,2,… the
 *            values are: 5*m+4 for m≠5k+4, and, when m≡4 (mod 5), the four
 *            consecutive numbers 5*m, 5*m+1, 5*m+2, 5*m+3. Equivalently:
 *            build blocks of 5 indices producing 8 terms: 5*(5k)+4,
 *            5*(5k+1)+4, 5*(5k+2)+4, 5*(5k+3)+4, 5*(5k+4), 5*(5k+4)+1, 5*...
 * Range:     i8 n <= 21, i16 n <= 21, i32 n <= 21, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t g   = (n - 1) / 8;        /* block number (zero‑based) */
    idx_t rem = (n - 1) % 8;        /* offset inside the block */

    if (rem < 4) {
        /* m = 5*g + rem  (rem = 0..3)  → 5*m+4 = 25*g + 5*rem + 4 */
        return (seq_t)(25 * g + 4 + 5 * rem);
    } else {
        /* m = 5*g + 4  (rem = 4..7)  → 5*m + (rem-4) = 25*g + 20 + (rem-4) */
        return (seq_t)(25 * g + 20 + (rem - 4));
    }
}
