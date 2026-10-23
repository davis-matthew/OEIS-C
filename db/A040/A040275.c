/*
 * A040275  Continued fraction for sqrt(293).
 * https://oeis.org/A040275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=17; for n>=1 the sequence repeats with period [8,1,1,8,34]
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[5] = { (seq_t)8, (seq_t)1, (seq_t)1, (seq_t)8, (seq_t)34 };
    if (n == 0)
        return (seq_t)17;
    return period[(n - 1) % 5];
}
