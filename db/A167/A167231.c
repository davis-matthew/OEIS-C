/*
 * A167231  Append three digits, each increasing by one modulo 10 from the
 *          last digit of the nonnegative integers. 0 -> 123, 1 -> 1234 2 ->
 *          2345, ... , 9 -> 9012, 10 -> 10123, etc.
 * https://oeis.org/A167231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n*1000 + ((n%10+1)%10)*100 + ((n%10+2)%10)*10 +
 *            ((n%10+3)%10)
 * Range:     u8 n <= 0, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t d = n % 10;                       /* last digit of n, 0 .. 9 */
    seq_t result = (seq_t)n * 1000u;        /* original number shifted left by 3 decimal digits */
    seq_t digits = ((seq_t)((d + 1) % 10)) * 100u
                 + ((seq_t)((d + 2) % 10)) * 10u
                 + ((seq_t)((d + 3) % 10));
    return result + digits;
}
