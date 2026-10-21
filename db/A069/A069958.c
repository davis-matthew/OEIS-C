/*
 * A069958  (Sum of digits of n)^3 - (sum of digits^3 of n).
 * https://oeis.org/A069958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (sum of digits of n)^3 - sum of (digit^3 of n)
 * Range:     u8 n <= 18, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t sumCubes = 0;
    idx_t tmp = n;
    /* Handle n = 0 correctly with do-while */
    do
    {
        unsigned int d = tmp % 10;
        sum += d;
        sumCubes += (seq_t)d * d * d;
        tmp /= 10;
    } while (tmp);
    return sum * sum * sum - sumCubes;
}
