/*
 * A082593  Values in Pfennigs of German money before the introduction of the
 *          Euro.
 * https://oeis.org/A082593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 1,2,5,10,50 for n=1..5; for n>=6 let k=(n-6)/3+1,
 *            r=(n-6)%3; a(n) = 10^k * [10,20,50][r]
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Validated: full, pass, 15 terms through n = 15 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

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
    if (n == 0) return (seq_t)0;            /* out of range, not used */
    if (n <= 5) {
        static const seq_t init[5] = {
            (seq_t)1,
            (seq_t)2,
            (seq_t)5,
            (seq_t)10,
            (seq_t)50
        };
        return init[n - 1];
    }
    idx_t offset = n - 6;
    idx_t k = offset / 3 + 1;
    unsigned int r = offset % 3;
    static const unsigned int mult[3] = {10, 20, 50};
    seq_t p10 = pow10(k);
    return mul_small(p10, mult[r]);
}
