/*
 * A060485  Number of 7-block tricoverings of an n-set.
 * https://oeis.org/A060485
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (35^n - 7*20^n - 21*15^n + 42*10^n + 105*8^n + 105*7^n +
 *            70*5^n - 945*4^n - 525*3^n + 2450*2^n - 1470) / 5040
 * Range:     u8 none, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   pow128_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow128_uint128.h"

/* Fast power for arbitrary‑precision seq_t. Uses only + - * /. */
static inline seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t res = 1;
    while (exp) {
        if (exp & 1) res = res * base;
        base = base * base;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n < 4) return (seq_t)0;   /* offset starts at n = 4 */

    const seq_t denom = 5040;     /* 7! */

    /* Use 128‑bit arithmetic when seq_t is 64‑bit or smaller (fits the noted
       values for n ≤ 14).  For larger seq_t we rely on its natural
       arithmetic handles arbitrary precision. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 pos = 0;
        unsigned __int128 neg = 0;

        /* Positive contributions */
        pos += pow128_uint128(35, n);
        pos += pow128_uint128(10, n) * 42;
        pos += pow128_uint128(8, n)  * 105;
        pos += pow128_uint128(7, n)  * 105;
        pos += pow128_uint128(5, n)  * 70;
        pos += pow128_uint128(2, n)  * 2450;

        /* Negative contributions */
        neg += pow128_uint128(20, n) * 7;
        neg += pow128_uint128(15, n) * 21;
        neg += pow128_uint128(4, n)  * 945;
        neg += pow128_uint128(3, n)  * 525;
        neg += 1470;

        unsigned __int128 num = pos - neg;
        return (seq_t)(num / denom);
    } else {
        seq_t pos = 0;
        seq_t neg = 0;

        /* Positive contributions */
        pos += pow_seq((seq_t)35, n);
        pos += pow_seq((seq_t)10, n) * 42;
        pos += pow_seq((seq_t)8, n)  * 105;
        pos += pow_seq((seq_t)7, n)  * 105;
        pos += pow_seq((seq_t)5, n)  * 70;
        pos += pow_seq((seq_t)2, n)  * 2450;

        /* Negative contributions */
        neg += pow_seq((seq_t)20, n) * 7;
        neg += pow_seq((seq_t)15, n) * 21;
        neg += pow_seq((seq_t)4, n)  * 945;
        neg += pow_seq((seq_t)3, n)  * 525;
        neg += 1470;

        seq_t num = pos - neg;
        return num / denom;
    }
}
