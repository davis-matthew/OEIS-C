/*
 * A068094  Number of n-digit triangular numbers.
 * https://oeis.org/A068094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   Count of n‑digit triangular numbers a(n) = #{ k ≥ 1 | 10^(n-1) ≤
 *            k(k+1)/2 ≤ 10^n – 1 }
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 27 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Generic power for small exponents (k <= 3) */
static inline seq_t pow_seq(seq_t a, int k)
{
    seq_t r = 1;
    while (k)
    {
        if (k & 1)
            r *= a;
        if (k >>= 1)
            a *= a;
    }
    return r;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (pow_seq(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/*--- 128‑bit helper: 10^e  ---------------------------------------------------*/
static unsigned __int128 _pow10_128(idx_t e)
{
    unsigned __int128 r = 1;
    for (idx_t i = 0; i < e; ++i) r *= 10;
    return r;
}

/*--- 128‑bit helper: ceil of the square root of a ≥ 0  ------------------------*/
static unsigned __int128 _sqrt128(unsigned __int128 x)
{
    unsigned __int128 r = 0;
    unsigned __int128 bit = ((unsigned __int128)1) << 126;
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;                /* floor(√x)  */
}

/*--- Main sequence function --------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* safety for out of range */

    if (sizeof(seq_t) <= 8) {            /* 64‑bit or smaller -- use 128‑bit tmp */
        unsigned __int128 pow_n_minus1 = _pow10_128(n - 1);
        unsigned __int128 pow_n       = pow_n_minus1 * 10;          /* 10^n   */

        /* L = 10^(n-1)  =>  D1 = 8*L + 1  */
        unsigned __int128 D1 = pow_n_minus1 * 8 + 1;
        unsigned __int128 s1 = _sqrt128(D1);          /* floor(√D1)  */
        unsigned __int128 k_min;
        if (s1 * s1 == D1)                      /* perfect square */
            k_min = (s1 - 1) / 2;
        else
            k_min = (s1 + 1) / 2;                /* ceil of (-1+√D1)/2 */

        /* R = 10^n – 1  =>  D2 = 8*R + 1 = 8*10^n – 7  */
        unsigned __int128 D2 = pow_n * 8 - 7;
        unsigned __int128 s2 = _sqrt128(D2);          /* floor(√D2)  */
        unsigned __int128 k_max = (s2 - 1) / 2;        /* floor of (-1+√D2)/2 */

        if (k_max < k_min) return (seq_t)0;

        return (seq_t)(k_max - k_min + 1);
    }

    /*---  arbitrary‑precision seq_t ------------------------------------------*/
    seq_t L = pow10(n - 1);                    /* 10^(n-1)  */
    seq_t R = pow10(n) - (seq_t)1;             /* 10^n – 1  */

    seq_t D1 = (seq_t)8 * L + (seq_t)1;        /* 8*L + 1   */
    seq_t s1 = floor_sqrt(D1);                 /* floor(√D1) */
    seq_t k_min = (s1 * s1 == D1) ? (s1 - (seq_t)1) / (seq_t)2
                                   : (s1 + (seq_t)1) / (seq_t)2;

    seq_t D2 = (seq_t)8 * R + (seq_t)1;        /* 8*R + 1   */
    seq_t s2 = floor_sqrt(D2);                 /* floor(√D2) */
    seq_t k_max = (s2 - (seq_t)1) / (seq_t)2;   /* floor(-1+√D2)/2 */

    if (k_max < k_min) return (seq_t)0;
    return k_max - k_min + (seq_t)1;
}
