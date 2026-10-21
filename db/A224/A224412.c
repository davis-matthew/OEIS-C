/*
 * A224412  Number of 5Xn 0..1 arrays with rows unimodal and antidiagonals
 *          nondecreasing.
 * https://oeis.org/A224412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/3628800)*n^10 + (1/80640)*n^9 + (1/3456)*n^8 +
 *            (23/5760)*n^7 + (7213/172800)*n^6 + (683/2304)*n^5 +
 *            (138869/36288)*n^4 + (32143/2240)*n^3 + (32909/450)*n^2 -
 *            (753/20)*n - 44 (for n > 3) The expression is equivalent to:
 *            a(n) = ( n^10 + 45*n^9 + 1050*n^8 + 14490*n^7 + 151473*n^6 +
 *            1075725...
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 36, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned __int128 pow_uint128(unsigned __int128 base, unsigned int exp)
{
    unsigned __int128 res = 1;
    while (exp)
    {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)32;
    if (n == (idx_t)2) return (seq_t)324;
    if (n == (idx_t)3) return (seq_t)1288;

    /* compute numerator using 128‑bit arithmetic */
    unsigned __int128 x = (unsigned __int128)n;
    unsigned __int128 pow10 = pow_uint128(x, 10);
    unsigned __int128 pow9  = pow_uint128(x, 9);
    unsigned __int128 pow8  = pow_uint128(x, 8);
    unsigned __int128 pow7  = pow_uint128(x, 7);
    unsigned __int128 pow6  = pow_uint128(x, 6);
    unsigned __int128 pow5  = pow_uint128(x, 5);
    unsigned __int128 pow4  = pow_uint128(x, 4);
    unsigned __int128 pow3  = pow_uint128(x, 3);
    unsigned __int128 pow2  = pow_uint128(x, 2);
    unsigned __int128 pow1  = x;

    unsigned __int128 num = 0;
    num += pow10;                                  // coeff 1
    num += ((unsigned __int128)45) * pow9;          // 45
    num += ((unsigned __int128)1050) * pow8;        // 1050
    num += ((unsigned __int128)14490) * pow7;       // 14490
    num += ((unsigned __int128)151473) * pow6;      // 151473
    num += ((unsigned __int128)1075725) * pow5;     // 1075725
    num += ((unsigned __int128)13886900) * pow4;    // 13,886,900
    num += ((unsigned __int128)52071660) * pow3;    // 52,071,660
    num += ((unsigned __int128)265378176) * pow2;   // 265,378,176
    num += ((unsigned __int128)(-136624320)) * pow1; // -136,624,320
    num += ((unsigned __int128)(-159667200));       // -159,667,200

    /* divide by 3628800 to get final value */
    return (seq_t)(num / 3628800);
}
