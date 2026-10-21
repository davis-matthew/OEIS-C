/*
 * A091337  a(n) = (2/n), where (k/n) is the Kronecker symbol.
 * https://oeis.org/A091337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = (2/n) Kronecker symbol
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic of length 8: 0,1,0,-1,0,-1,0,1 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* a(0) = 0 */
    if ((n & 1ULL) == 0) return 0;       /* even n → 0 */

    /* n is odd: only classes 1,7 → +1, 3,5 → -1 */
    switch (n & 7ULL) {
        case 1: case 7: return (seq_t)1;
        default: return (seq_t)-1;
    }
}
