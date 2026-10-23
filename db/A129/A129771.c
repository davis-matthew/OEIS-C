/*
 * A129771  Evil odd numbers.
 * https://oeis.org/A129771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 2*A000069(n)+1 (odd evil numbers)
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but guard for safety */

    seq_t x = (seq_t)3;            /* first odd evil number */
    idx_t count = 0;

    for (;;)
    {
        /* compute popcount of x */
        unsigned int pc = 0;
        seq_t y = x;
        while (y)
        {
            pc += (unsigned)(y & (seq_t)1);
            y >>= 1;
        }

        if ((pc & 1) == 0)         /* even number of 1‑bits */
        {
            count++;
            if (count == n) return x;
        }

        x += (seq_t)2;             /* move to next odd */
    }
}
