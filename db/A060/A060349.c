/*
 * A060349  a(n) = n^(n+2)*(n^2 - 1)*(n+3)*(n+2)*(5*n - 7)/5760.
 * https://oeis.org/A060349
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n+2)*(n^2 - 1)*(n+3)*(n+2)*(5*n - 7)/5760 (n ≥ 3)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

/* Small‑integer power with 128‑bit intermediate (used when seq_t is ≤ 8 bytes) */
static __uint128_t pow_uint128(__uint128_t base, uint64_t exp)
{
    __uint128_t result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;

    /* ----------------- 128‑bit path for built‑in integer types ----------------- */
#if defined(__SIZEOF_INT128__)
    if (sizeof(seq_t) <= 8) {
        uint64_t lu = (uint64_t)n;          /* the index as a native integer */
        __uint128_t powp = pow_uint128((__uint128_t)lu, lu + 2ULL);
        __uint128_t num = powp;
        num *= ( (__uint128_t)(lu * lu - 1ULL) );
        num *= ( (__uint128_t)(lu + 3ULL) );
        num *= ( (__uint128_t)(lu + 2ULL) );
        num *= ( (__uint128_t)(5ULL * lu - 7ULL) );
        num /= 5760ULL;
        return (seq_t)num;
    }
#endif

    /* ----------------- generic big‑integer path ----------------- */
    const seq_t nn   = (seq_t)n;
    const seq_t powp = big_pow(nn, nn + (seq_t)2);

    seq_t num = powp;
    num *= (nn * nn - (seq_t)1);
    num *= (nn + (seq_t)3);
    num *= (nn + (seq_t)2);
    num *= (5 * nn - (seq_t)7);
    return num / (seq_t)5760;
}
