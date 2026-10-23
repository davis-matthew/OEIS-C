/*
 * A105734  Duplicate of A076839.
 * https://oeis.org/A105734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 1 for n≡1,2 (mod 5); a(n) = 2 for n≡0,3 (mod 5); a(n) = 3
 *            for n≡4 (mod 5)
 * Range:     i8 n <= 22, i16 n <= 22, i32 n <= 22, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Period‑5 pattern: {2,1,1,2,3} corresponding to n mod 5 = 0..4  */
    static const seq_t table[5] = { 2, 1, 1, 2, 3 };
    return table[n % 5];
}
