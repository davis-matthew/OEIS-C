/*
 * A040804  Continued fraction for sqrt(833).
 * https://oeis.org/A040804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=28, a(n)=pattern[(n-1)%14] for n>0 * where pattern is the
 *            repeating cycle of the continued fraction * for sqrt(833): [28;
 *            (1,6,4,3,2,1,2,1,2,3,4,6,1,56)]
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic part of the continued fraction for sqrt(833) */
    static const seq_t pattern[14] = {
        1, 6, 4, 3, 2, 1,
        2, 1, 2, 3, 4, 6,
        1, 56
    };

    if (n == 0)
        return 28;

    /* (n-1) is safe because n>0 in this branch */
    idx_t pos = (n - 1) % 14u;
    return pattern[pos];
}
