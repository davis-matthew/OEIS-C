/*
 * A234355  Decimal expansion of B(16) = -3617/510, the 16th Bernoulli number.
 * https://oeis.org/A234355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(1) = 7; a(2) = 0; for n >= 3, a(n) = d[(n-3) % 16] where d =
 *            {9,2,1,5,6,8,6,2,7,4,5,0,9,8,0,3} (digits of the decimal
 *            expansion of the 16th Bernoulli number B(16) = -3617/510)
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1U)
        return (seq_t)7;
    if (n == 2U)
        return (seq_t)0;

    static const seq_t cycle[16] = {
        (seq_t)9, (seq_t)2, (seq_t)1, (seq_t)5,
        (seq_t)6, (seq_t)8, (seq_t)6, (seq_t)2,
        (seq_t)7, (seq_t)4, (seq_t)5, (seq_t)0,
        (seq_t)9, (seq_t)8, (seq_t)0, (seq_t)3
    };

    return cycle[(n - 3U) % 16U];
}
