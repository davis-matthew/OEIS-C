/*
 * A104175  From the words to the song "867-5309/Jenny" by Tommy Tutone.
 * https://oeis.org/A104175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, dumb
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Pattern: a(n) = [8,6,7,5,3,0,9][(n-1) mod 7] */
/* Derived from (8 + 6*x + 7*x^2 + 5*x^3 + 3*x^4 + 9*x^6)/(1 - x^7) */
seq_t a(idx_t n)
{
    static const seq_t pattern[7] = {
        (seq_t)8, (seq_t)6, (seq_t)7,
        (seq_t)5, (seq_t)3, (seq_t)0,
        (seq_t)9
    };
    /* n-1 is safe for n >= 1 */
    idx_t idx = (n - 1) % 7;
    return pattern[idx];
}
