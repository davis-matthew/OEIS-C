/*
 * A118667  a(n) = a(n-1)+ (abs(2^a(n-1)*a(n-1)) mod 10).
 * https://oeis.org/A118667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + (abs(2^a(n-1)*a(n-1)) mod 10) with initial
 *            values a(0) = 0, a(1) = 1
 * Range:     u8 n <= 64, u16 n <= 250, u32 n <= 250, u64 n <= 250,
 *            u128 n <= 250, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t cur = (seq_t)1;           /* a(1) */

    for (idx_t i = 2 ; i <= n ; ++i)
    {
        /* compute 2^cur mod 10 */
        seq_t pow2;
        if (cur == 0)
            pow2 = (seq_t)1;
        else
        {
            unsigned int r = (unsigned int)(cur % 4);
            switch (r)
            {
                case 1: pow2 = (seq_t)2; break;
                case 2: pow2 = (seq_t)4; break;
                case 3: pow2 = (seq_t)8; break;
                default: /* r == 0 */ pow2 = (seq_t)6; break;
            }
        }

        /* compute the addend (pow2 * cur) mod 10 */
        seq_t add = (pow2 * (cur % 10)) % 10;

        cur += add;
    }

    return cur;
}
