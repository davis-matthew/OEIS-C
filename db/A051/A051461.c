/*
 * A051461  Molien series for group G_{1,3} of order 2304.
 * https://oeis.org/A051461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 20, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A051461 – Molien series for group G_{1,3} of order 2304.
 * 
 * a(n) = floor(
 *    (24*n^5+90*n^4+135*n^3+495*n^2+116*n+1620)/1620
 *  - (n mod 2)*(n^2-n+6)/12
 *  + (77-54*(n mod 2))*((n+2) mod 3)*n/162
 *  + ((n+1) mod 3)*(4*n^3+9*n)/162
 *  - (n mod 3)*(2*n^3+18*n^2+25*n+18)/81
 * );
 *
 * The implementation evaluates each term with integer arithmetic,
 * multiplies numerators by a common denominator (1620),
 * and performs the final division.  All calculations use
 * unsigned __int128 for intermediate results to avoid overflow
 * for n up to at least 10,000.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* cast n to __uint128_t for safe multiplications */
    __uint128_t nn = n;
    __uint128_t n2 = nn * nn;
    __uint128_t n3 = n2 * nn;
    __uint128_t n4 = n3 * nn;
    __uint128_t n5 = n4 * nn;

    /* ---------------- Term 1 ---------------- */
    __uint128_t term1 = 24 * n5 + 90 * n4 + 135 * n3 + 495 * n2
                       + 116 * nn + (__uint128_t)1620;

    /* ---------------- Term 2 ---------------- */
    __uint128_t term2 = 0;
    if ((n & 1) != 0)                     /* n mod 2 == 1 */
    {
        __uint128_t t = (n * n - nn + 6);   /* n^2 - n + 6 */
        term2 = t * 135;                    /* factor 1620/12 = 135 */
    }

    /* ---------------- Term 3 ---------------- */
    __uint128_t term3 = 0;
    {
        uint64_t coeff = 77 - 54 * (n & 1); /* 77 or 23 depending on parity */
        uint64_t mod_n2 = (n + 2) % 3;      /* (n+2) mod 3 */
        term3 = (__uint128_t)coeff * (__uint128_t)mod_n2 * nn * 10; /* *1620/162 = 10 */
    }

    /* ---------------- Term 4 ---------------- */
    __uint128_t term4 = 0;
    {
        uint64_t mod_n1 = (n + 1) % 3;      /* (n+1) mod 3 */
        if (mod_n1 != 0)
        {
            __uint128_t inner = 4 * n3 + 9 * nn; /* 4*n^3 + 9*n */
            term4 = (__uint128_t)mod_n1 * inner * 10; /* *1620/162 = 10 */
        }
    }

    /* ---------------- Term 5 ---------------- */
    __uint128_t term5 = 0;
    {
        uint64_t mod_n3 = n % 3;            /* n mod 3 */
        if (mod_n3 != 0)
        {
            __uint128_t inner = 2 * n3 + 18 * n2 + 25 * nn + 18;
            term5 = (__uint128_t)mod_n3 * inner * 20; /* *1620/81 = 20 */
        }
    }

    /* Combine all terms with the common denominator 1620 */
    __uint128_t N = term1;
    N -= term2;
    N += term3;
    N += term4;
    N -= term5;

    /* Exact division; integer truncation gives floor for non‑negative N */
    return (seq_t)(N / 1620);
}
