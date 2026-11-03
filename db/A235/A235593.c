/*
 * A235593  Binomial(n-1,3)+3*binomial(n-1,4)+6*binomial(n-1,5)+5*binomial(n-1,6).
 * https://oeis.org/A235593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            binomial(n-1,3)+3*binomial(n-1,4)+6*binomial(n-1,5)+5*binomial(n-1,6)
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

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
    if (n < 4) return (seq_t)0;

    seq_t res = binomial(n - 1, 3);

    if (n >= 5) {
        seq_t t = binomial(n - 1, 4);
        res += mul_small(t, 3);
    }
    if (n >= 6) {
        seq_t t = binomial(n - 1, 5);
        res += mul_small(t, 6);
    }
    if (n >= 7) {
        seq_t t = binomial(n - 1, 6);
        res += mul_small(t, 5);
    }

    return res;
}
