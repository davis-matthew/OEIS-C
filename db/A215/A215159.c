/*
 * A215159  a(n) = floor(n^n / (n+1)).
 * https://oeis.org/A215159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⌊ n^n / (n+1) ⌋
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* 64‑bit safe implementation (uses unsigned __int128) for the
    built‑in integral seq_t types.  For arbitrary‑precision types
    the directly defined multiplication is used. */
seq_t a(idx_t n) {
    /* n = 0 → 0^0 is treated as 1, division by 1 gives 1 */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* If seq_t is an ordinary unsigned integer (≤ 8 bytes) we
       compute using 128‑bit intermediate to avoid overflow. */
    if (sizeof(seq_t) <= 8) {
        __uint128_t base = (unsigned long long)n;
        __uint128_t exp  = (unsigned long long)n;
        __uint128_t power = 1;
        while (exp) {
            if (exp & 1)
                power *= base;
            base *= base;
            exp >>= 1;
        }
        __uint128_t denom = (unsigned long long)n + 1;
        return (seq_t)(power / denom);
    }

    /* Generic big‑integer case – multiplication is handled by
       the seq_t implementation. */
    seq_t base = (seq_t)n;
    seq_t exp  = (seq_t)n;
    seq_t power = 1;
    while (exp) {
        if (exp & 1)
            power = power * base;
        base = base * base;
        exp >>= 1;
    }
    seq_t denom = (seq_t)n + 1;
    return power / denom;
}
