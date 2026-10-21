/*
 * A222126  Number of binary arrays indicating the locations of trailing edge
 *          maxima of a random length-n 0..7 array extended with zeros and
 *          convolved with 1,2,1.
 * https://oeis.org/A222126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-5) + 3*a(n-7) - 2*a(n-8) - a(n-10)
 *            - a(n-12) - 2*a(n-14) + a(n-15). Initial values: a(1)=2, a(2)=3,
 *            a(3)=4, a(4)=8, a(5)=12, a(6)=19, a(7)=31, a(8)=49, a(9)=80,
 *            a(10)=128, a(11)=207, a(12)=334, a(13)=536, a(14)=866,
 *            a(15)=1394
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* unused, offset is 1 */

    switch (n)
    {
        case 1:  return 2;
        case 2:  return 3;
        case 3:  return 4;
        case 4:  return 8;
        case 5:  return 12;
        case 6:  return 19;
        case 7:  return 31;
        case 8:  return 49;
        case 9:  return 80;
        case 10: return 128;
        case 11: return 207;
        case 12: return 334;
        case 13: return 536;
        case 14: return 866;
        case 15: return 1394;
        default:
        {
            seq_t buf[16];

            /* initialise the first 15 values */
            buf[1] = 2;
            buf[2] = 3;
            buf[3] = 4;
            buf[4] = 8;
            buf[5] = 12;
            buf[6] = 19;
            buf[7] = 31;
            buf[8] = 49;
            buf[9] = 80;
            buf[10] = 128;
            buf[11] = 207;
            buf[12] = 334;
            buf[13] = 536;
            buf[14] = 866;
            buf[15] = 1394;

            for (idx_t i = 16; i <= n; ++i)
            {
                seq_t val =
                    buf[(i - 1) % 16] + buf[(i - 3) % 16] + buf[(i - 5) % 16] +
                    (3 * buf[(i - 7) % 16]) -
                    (2 * buf[(i - 8) % 16]) - buf[(i - 10) % 16] -
                    buf[(i - 12) % 16] - (2 * buf[(i - 14) % 16]) +
                    buf[(i - 15) % 16];

                buf[i % 16] = val;
            }

            return buf[n % 16];
        }
    }
}
