/*
 * A189388  a(n) = n+[nr/t]+[ns/t]; r=1, s=sqrt(2), t=1/sqrt(3).
 * https://oeis.org/A189388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*sqrt(3)) + floor(n*sqrt(6))
 * Range:     u8 n <= 49, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 128‑bit unsigned integer.
 * Returns floor(sqrt(x)).  The algorithm is the classical
 * binary‑digit extraction method which uses only integer
 * arithmetic and works for any size up to 128 bits. */
static unsigned __int128 sqrt128(unsigned __int128 x)
{
    unsigned __int128 res = 0;
    unsigned __int128 bit = 1;

    /* Find the greatest power of four <= x. */
    while (bit <= (x >> 2))
        bit <<= 2;

    for (; bit != 0; bit >>= 2)
    {
        unsigned __int128 tmp = res + bit;
        if (x >= tmp)
        {
            x -= tmp;
            res = (res >> 1) + bit;
        }
        else
        {
            res >>= 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for the intermediate products to
     * avoid overflow that would occur with the small types
     * used for seq_t or idx_t. */
    unsigned __int128 nn = (unsigned __int128)n;

    unsigned __int128 prod3 = nn * nn * 3;   /* n²·3 = (n·√3)²  */
    unsigned __int128 prod6 = nn * nn * 6;   /* n²·6 = (n·√6)²  */

    unsigned __int128 s3 = sqrt128(prod3);
    unsigned __int128 s6 = sqrt128(prod6);

    unsigned __int128 res = nn + s3 + s6;

    return (seq_t)res;   /* cast to the user‑provided seq_t type */
}
