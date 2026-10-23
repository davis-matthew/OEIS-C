/*
 * A101201  Maximal number of kings in the toroidal king's graph on an n X n
 *          board such that each king is attacking no more than four other
 *          kings.
 * https://oeis.org/A101201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = ⌊3*n*n/5⌋ – f(n), where f(n) = 0 if n mod 5 = 0, ⌊n/5⌋–1
 *            if n mod 5 = 1 and n>1, ⌊n/5⌋ otherwise
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1 (input assumption) */
    idx_t      k = n / 5;       /* floor(n/5)  */
    idx_t      r = n % 5;       /* n mod 5     */

    seq_t nn = (seq_t)n;        /* promote n to seq_t for multiplication */
    seq_t term = (nn * nn * 3) / 5;   /* floor(3 n^2 / 5) */

    seq_t subtract;
    if (r == 0) {
        subtract = 0;
    } else if (r == 1) {
        /* for n = 1 => subtract = 0, otherwise subtract = k-1 */
        subtract = (k > 0) ? (seq_t)(k - 1) : (seq_t)0;
    } else {
        subtract = (seq_t)k;
    }

    return term - subtract;
}
