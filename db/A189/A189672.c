/*
 * A189672  Partial sums of A080846.
 * https://oeis.org/A189672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+1)/3) + a(floor(n/3)) ; a(0)=0
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    while (n > 0)
    {
        sum += (seq_t)((n + 1) / 3);
        n = n / 3;
    }
    return sum;
}
