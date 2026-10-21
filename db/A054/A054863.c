/*
 * A054863  Dice markers in the board game "The Settlers of Catan". There are
 *          a total of 18 markers, whose backs are assigned letters from A to
 *          R and whose fronts are assigned these numbers.
 * https://oeis.org/A054863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Dice markers in the board game “The Settlers of Catan”.
   The sequence consists of the numbers on the 18 dice‑marker tokens
   arranged in alphabetical order (A,B,…,R).  The list of values is
   defined in the game and is the only way to determine a(n); there is
   no closed‑form formula.  For 1 ≤ n ≤ 18 the value is the given number,
   and a(n) = 0 for n > 18 (or n = 0). */
seq_t a(idx_t n)
{
    static const seq_t markers[18] = {
        (seq_t)5,  (seq_t)2,  (seq_t)6,  (seq_t)3,  (seq_t)8,
        (seq_t)10, (seq_t)9,  (seq_t)12, (seq_t)11, (seq_t)4,
        (seq_t)8,  (seq_t)10, (seq_t)9,  (seq_t)4,  (seq_t)5,
        (seq_t)6,  (seq_t)3,  (seq_t)11
    };
    if (n == 0 || n > 18) return (seq_t)0;
    return markers[n - 1];
}
