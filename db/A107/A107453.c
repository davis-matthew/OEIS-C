/*
 * A107453  1 followed by repetitions of the period-4 sequence 1,1,1,2.
 * https://oeis.org/A107453
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for all n except when n%4==0 and n≥8 then a(n)=2; a(4)
 *            is also 1. (Offset: 4)
 * Range:     u8 n <= 108, u16 n <= 108, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)0;          /* values below the offset are undefined */
    if (n == 4)
        return (seq_t)1;
    /* n >= 5 */
    return ((n & 3ULL) == 0ULL) ? (seq_t)2 : (seq_t)1;
}
