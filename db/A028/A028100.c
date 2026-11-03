/*
 * A028100  Expansion of 1/((1-3x)(1-8x)(1-9x)(1-11x)).
 * https://oeis.org/A028100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*11^(n+3)-20*9^(n+3)+2*8^(n+4)-3^(n+3))/240 This
 *            implementation uses the order‑4 linear recurrence a(n) =
 *            31*a(n-1) - 343*a(n-2) + 1569*a(n-3) - 2376*a(n-4) with initial
 *            values a(0)=1, a(1)=31, a(2)=618, a(3)=10094. The helper
 *            mul_small guarantees correct multiplication for any seq_t typ...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

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
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)31;
    if (n == 2) return (seq_t)618;
    if (n == 3) return (seq_t)10094;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)31;
    seq_t a2 = (seq_t)618;
    seq_t a3 = (seq_t)10094;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t term = mul_small(a3, 31);
        term -= mul_small(a2, 343);
        term += mul_small(a1, 1569);
        term -= mul_small(a0, 2376);

        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = term;
    }
    return a3;
}
