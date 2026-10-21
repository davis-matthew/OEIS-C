/*
 * A060101  Sixth column (m=5) of triangle A060098.
 * https://oeis.org/A060101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (14175*(30827+1941*(-1)^n) + 1440*(676427+11445*(-1)^n)*n
 *            + 126*(6861329+27375*(-1)^n)*n^2 + 1600*(258451+189*(-1)^n)*n^3
 *            + 10*(12016607+945*(-1)^n)*n^4 + 22444800*n^5 + 2754192*n^6 +
 *            220800*n^7 + 11130*n^8 + 320*n^9 + 4*n^10)/ 464486400
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 51, u64 n <= 532,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first 16 terms of the sequence */
    static const seq_t init[16] = {
        1ULL, 6ULL, 26ULL, 86ULL, 246ULL,
        622ULL, 1442ULL, 3102ULL, 6292ULL, 12122ULL,
        22374ULL, 39754ULL, 68354ULL, 114114ULL,
        185614ULL, 294866ULL
    };

    if (n < 16)
        return init[n];

    seq_t buf[16];
    for (int i = 0; i < 16; ++i)
        buf[i] = init[i];

    for (idx_t i = 16; i <= n; ++i)
    {
        unsigned __int128 sum = 0;
        sum += (unsigned __int128)buf[15] * 6;
        sum -= (unsigned __int128)buf[14] * 10;
        sum -= (unsigned __int128)buf[13] * 10;
        sum += (unsigned __int128)buf[12] * 50;
        sum -= (unsigned __int128)buf[11] * 34;
        sum -= (unsigned __int128)buf[10] * 66;
        sum += (unsigned __int128)buf[9]  * 110;
        /* buf[8] term is 0 */
        sum -= (unsigned __int128)buf[7]  * 110;
        sum += (unsigned __int128)buf[6]  * 66;
        sum += (unsigned __int128)buf[5]  * 34;
        sum -= (unsigned __int128)buf[4]  * 50;
        sum += (unsigned __int128)buf[3]  * 10;
        sum += (unsigned __int128)buf[2]  * 10;
        sum -= (unsigned __int128)buf[1]  * 6;
        sum += (unsigned __int128)buf[0]  * 1;
        seq_t next = (seq_t)sum;

        /* shift the buffer */
        for (int k = 0; k < 15; ++k)
            buf[k] = buf[k + 1];
        buf[15] = next;
    }

    return buf[15];
}
