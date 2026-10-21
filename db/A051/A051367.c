/*
 * A051367  Number of 5-element families of an n-element set such that every 4
 *          members of the family have a nonempty intersection.
 * https://oeis.org/A051367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/120)*(32^n - 5*30^n + 10*29^n - 10*28^n + 5*27^n -
 *            26^n * - 10*16^n + 10*15^n + 35*8^n - 35*7^n - 50*4^n + 50*3^n *
 *            + 24*2^n - 24)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: the function works for any seq_t.  For small
 * seq_t (≤ 64 bits) it uses a 128‑bit intermediate to avoid
 * overflow until the result fits in 'seq_t'.  For larger seq_t
 * (arbitrary‑precision) it uses the provided big_pow routine. */

#include <stdint.h>
#include <stdbool.h>

#if !defined(_WIN32) && !defined(_MSC_VER)
/* big_pow is supplied in the utilities for arbitrary‑precision builds */
#include "../utils/big_pow.h"
#endif

/* 128‑bit power for small seq_t types */
static inline __int128 pow128(uint64_t base, idx_t exp)
{
    __int128 result = 1;
    __int128 b = base;
    while (exp > 0)
    {
        if (exp & 1ULL) result *= b;
        b *= b;
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)0;

    /* Choose implementation based on the size of seq_t */
    if (sizeof(seq_t) <= 8)      /* small integral types */
    {
        /* All intermediate calculations use __int128 */
        __int128 p32 = pow128(32, n);
        __int128 p30 = pow128(30, n);
        __int128 p29 = pow128(29, n);
        __int128 p28 = pow128(28, n);
        __int128 p27 = pow128(27, n);
        __int128 p26 = pow128(26, n);
        __int128 p16 = pow128(16, n);
        __int128 p15 = pow128(15, n);
        __int128 p8  = pow128(8,  n);
        __int128 p7  = pow128(7,  n);
        __int128 p4  = pow128(4,  n);
        __int128 p3  = pow128(3,  n);
        __int128 p2  = pow128(2,  n);

        __int128 numer =
              p32
            - (__int128)5 * p30
            + (__int128)10 * p29
            - (__int128)10 * p28
            + (__int128)5 * p27
            - p26
            - (__int128)10 * p16
            + (__int128)10 * p15
            + (__int128)35 * p8
            - (__int128)35 * p7
            - (__int128)50 * p4
            + (__int128)50 * p3
            + (__int128)24 * p2
            - (__int128)24;

        return (seq_t)(numer / 120);
    }
    else    /* arbitrary‑precision seq_t */
    {
        seq_t p32 = big_pow((seq_t)32, (seq_t)n);
        seq_t p30 = big_pow((seq_t)30, (seq_t)n);
        seq_t p29 = big_pow((seq_t)29, (seq_t)n);
        seq_t p28 = big_pow((seq_t)28, (seq_t)n);
        seq_t p27 = big_pow((seq_t)27, (seq_t)n);
        seq_t p26 = big_pow((seq_t)26, (seq_t)n);
        seq_t p16 = big_pow((seq_t)16, (seq_t)n);
        seq_t p15 = big_pow((seq_t)15, (seq_t)n);
        seq_t p8  = big_pow((seq_t)8,  (seq_t)n);
        seq_t p7  = big_pow((seq_t)7,  (seq_t)n);
        seq_t p4  = big_pow((seq_t)4,  (seq_t)n);
        seq_t p3  = big_pow((seq_t)3,  (seq_t)n);
        seq_t p2  = big_pow((seq_t)2,  (seq_t)n);

        seq_t res = p32;
        res -= p30 * (seq_t)5;
        res += p29 * (seq_t)10;
        res -= p28 * (seq_t)10;
        res += p27 * (seq_t)5;
        res -= p26;
        res -= p16 * (seq_t)10;
        res += p15 * (seq_t)10;
        res += p8  * (seq_t)35;
        res -= p7  * (seq_t)35;
        res -= p4  * (seq_t)50;
        res += p3  * (seq_t)50;
        res += p2  * (seq_t)24;
        res -= (seq_t)24;
        res /= (seq_t)120;
        return res;
    }
}
