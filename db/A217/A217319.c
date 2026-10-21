/*
 * A217319  Numbers with binary representation ending in 4*k+2 or 4*k+3 zeros.
 * https://oeis.org/A217319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth positive integer whose binary representation ends
 *            with a * block of 2 or 3 mod 4 trailing zeros
 * Range:     u8 n <= 51, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t num = 1;           /* current integer being tested          */
    idx_t  count = 0;        /* how many qualifying numbers seen      */

    /* iterate over positive integers until we find the nth qualifying one */
    for (;; ++num)
    {
        /* count trailing zeros of num  */
        idx_t tz = 0;
        seq_t tmp = num;

        while ((tmp & (seq_t)1) == 0)
        {
            ++tz;
            tmp >>= 1;
        }

        /* check if the count of trailing zeros is 2 or 3 modulo 4 */
        if ((tz & 3) == 2 || (tz & 3) == 3)
        {
            ++count;
            if (count == n)
                return num;
        }
    }
}
