/*
 * A040429  Continued fraction for sqrt(451).
 * https://oeis.org/A040429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   continued fraction for sqrt(451). a(0)=21; a(n)=period[(n-1)%10]
 *            for n>0
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* period: 4,4,2,8,21,8,2,4,4,42 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)21;

    static const seq_t period[10] = {
        (seq_t)4, (seq_t)4, (seq_t)2, (seq_t)8, (seq_t)21,
        (seq_t)8, (seq_t)2, (seq_t)4, (seq_t)4, (seq_t)42
    };

    idx_t pos = (n - 1) % 10;
    return period[pos];
}
