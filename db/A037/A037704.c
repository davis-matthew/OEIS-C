/*
 * A037704  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,3,0,2.
 * https://oeis.org/A037704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=1; a(n)=a(n-1)*6 + d_{n-1}, where d_k == 1,3,0,2 for
 *            k%4==0,1,2,3 respectively
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t result = 1;          /* a(1) */
    for (idx_t i = 1; i < n; ++i)
    {
        idx_t r = i & 3;          /* same as i % 4 */
        seq_t digit;
        switch (r)
        {
            case 0: digit = 1; break;   /* k%4==0 -> digit 1 */
            case 1: digit = 3; break;   /* k%4==1 -> digit 3 */
            case 2: digit = 0; break;   /* k%4==2 -> digit 0 */
            default: digit = 2; break;  /* k%4==3 -> digit 2 */
        }
        result = result * 6 + digit;
    }
    return result;
}
