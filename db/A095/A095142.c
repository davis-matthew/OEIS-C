/*
 * A095142  Triangle, read by rows, formed by reading Pascal's triangle
 *          (A007318) mod 7.
 * https://oeis.org/A095142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula as defined in OEIS A095142:
 * a(n) = binomial(i, j) mod 7, where i is the row number
 * of Pascal's triangle and j the column number such that
 * n = i*(i+1)/2 + j (0‑based). */

/*  All static helper functions and tables are defined below;
 *  the only required function exported by this file is
 *  a(idx_t n).  The code is valid C11 and C++17. */

#include <stdint.h>

/* ------------------------------------------------------------------
 * Local integer square‑root for 128‑bit values
 * ------------------------------------------------------------------ */
static idx_t isqrt_u128(__int128 x)
{
    idx_t low = 0, high = ((idx_t)1 << 32);   /* sqrt(8·2^64+1) < 2^32.5 */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        __int128 prod = (__int128)mid * mid;
        if (prod <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* ------------------------------------------------------------------
 * Binomial coefficient modulo 7 using Lucas' theorem
 * ------------------------------------------------------------------ */
static const uint8_t binom_mod7[7][7] = {
    {1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0},
    {1,2,1,0,0,0,0},
    {1,3,3,1,0,0,0},
    {1,4,6,4,1,0,0},
    {1,5,3,3,5,1,0},
    {1,6,1,6,1,6,1}
};

static seq_t binom_mod7_lucas(idx_t n, idx_t k)
{
    seq_t res = 1;
    while (n || k) {
        uint8_t ni = (uint8_t)(n % 7);
        uint8_t ki = (uint8_t)(k % 7);
        if (ki > ni) return 0;
        res = (res * binom_mod7[ni][ki]) % 7;
        n /= 7;
        k /= 7;
    }
    return res;
}

/* ------------------------------------------------------------------
 * Main sequence function
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Find the row index i from n:
     * n = i*(i+1)/2 + j   with 0 <= j <= i.
     * Solve i from the quadratic equation. */
    __int128 t = (__int128)8 * n + 1;          /* 8n+1 fits in 128 bit   */
    idx_t sq  = isqrt_u128(t);                /* floor(sqrt(8n+1))      */
    idx_t i   = (sq - 1) >> 1;                 /* floor((sqrt(8n+1)-1)/2) */

    /* Starting index of row i */
    idx_t start = i * (i + 1) / 2;
    idx_t j = n - start;                       /* 0‑based column within row */

    return binom_mod7_lucas(i, j);
}
