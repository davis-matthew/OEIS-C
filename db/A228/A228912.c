/*
 * A228912  a(n) = 10^n - 9*9^n + 36*8^n - 84*7^n + 126*6^n - 126*5^n + 84*4^n
 *          - 36*3^n + 9*2^n - 1.
 * https://oeis.org/A228912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n - 9*9^n + 36*8^n - 84*7^n + 126*6^n * - 126*5^n +
 *            84*4^n - 36*3^n + 9*2^n - 1
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 11, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 881 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ipow.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    seq_t pos = ipow((seq_t)10, (seq_t)n);
    seq_t t;

    t = ipow((seq_t)8, (seq_t)n);
    pos += mul_small(t, 36);

    t = ipow((seq_t)6, (seq_t)n);
    pos += mul_small(t, 126);

    t = ipow((seq_t)4, (seq_t)n);
    pos += mul_small(t, 84);

    t = ipow((seq_t)2, (seq_t)n);
    pos += mul_small(t, 9);

    seq_t neg = mul_small(ipow((seq_t)9, (seq_t)n), 9);
    neg += mul_small(ipow((seq_t)7, (seq_t)n), 84);
    neg += mul_small(ipow((seq_t)5, (seq_t)n), 126);
    neg += mul_small(ipow((seq_t)3, (seq_t)n), 36);
    neg += 1;

    return pos - neg;
}
