/*
 * A239814  Number of nX3 0..3 arrays with no element equal to zero plus the
 *          sum of elements to its left or one plus the sum of elements above
 *          it, modulo 4.
 * https://oeis.org/A239814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A239814
   Number of n×3 0..3 arrays with no element equal to
   the sum of elements to its left nor the sum of elements above
   it plus one, modulo 4.
   This implementation counts such arrays by dynamic programming
   over the 4³ = 64 possible column‑sum states.  */
#include <stdint.h>

#define STATE_COUNT 64U          /* 4³ */

static inline idx_t encode_state(uint8_t s0, uint8_t s1, uint8_t s2)
{
    return ((idx_t)s0 << 4) | ((idx_t)s1 << 2) | (idx_t)s2;
}

static inline void decode_state(idx_t code, uint8_t *s0, uint8_t *s1, uint8_t *s2)
{
    *s2 = code & 3U;
    *s1 = (code >> 2) & 3U;
    *s0 = (code >> 4) & 3U;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not needed for the official sequence */

    seq_t dp[STATE_COUNT] = {0};
    dp[0] = 1;                     /* initial state: all column sums are 0 */
    seq_t dp_next[STATE_COUNT];

    for (idx_t row = 1; row <= n; ++row) {
        for (idx_t i = 0; i < STATE_COUNT; ++i) dp_next[i] = 0;

        for (idx_t from = 0; from < STATE_COUNT; ++from) {
            if (dp[from] == 0) continue;

            uint8_t s0, s1, s2;
            decode_state(from, &s0, &s1, &s2);

            /* enumerate all admissible triplets (a, b, c) */
            for (uint8_t a = 0; a <= 3; ++a) {
                if (a == 0) continue;
                if (a == (1U + s0) % 4U) continue;

                for (uint8_t b = 0; b <= 3; ++b) {
                    if (b == a) continue;
                    if (b == (1U + s1) % 4U) continue;

                    for (uint8_t c = 0; c <= 3; ++c) {
                        uint8_t left2 = (a + b) % 4U;
                        if (c == left2) continue;
                        if (c == (1U + s2) % 4U) continue;

                        uint8_t ns0 = (s0 + a) % 4U;
                        uint8_t ns1 = (s1 + b) % 4U;
                        uint8_t ns2 = (s2 + c) % 4U;
                        idx_t to = encode_state(ns0, ns1, ns2);
                        dp_next[to] += dp[from];
                    }
                }
            }
        }

        for (idx_t i = 0; i < STATE_COUNT; ++i) dp[i] = dp_next[i];
    }

    seq_t total = 0;
    for (idx_t i = 0; i < STATE_COUNT; ++i) total += dp[i];
    return total;
}
