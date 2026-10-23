/*
 * A037558  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,1.
 * https://oeis.org/A037558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum_{k=0}^{n-1} ( (k%3==0)? 2 : 1 ) * 9^{n-1-k}
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to the recurrence: a(1)=2, a(n)=a(n-1)*9 + ( (n-1)%3==0 ? 2 : 1 ) for n>1 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;     /* offset is 1; a(0) is undefined, return 0 */

    seq_t result = (seq_t)0;
    for (idx_t i = 0; i < n; ++i)
    {
        seq_t digit = ( (i % 3) == 0 ? (seq_t)2 : (seq_t)1 );
        result = result * (seq_t)9 + digit;
    }
    return result;
}
