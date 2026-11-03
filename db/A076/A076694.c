/*
 * A076694  a(n) = n - sum of the distinct prime factors of n.
 * https://oeis.org/A076694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = n - sum of the distinct prime factors of n
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 80 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- factorise n into distinct prime factors (array of up to 32) ---------- */
static void factor_distinct_uint64(uint64_t n, uint64_t fac[32], size_t *cnt)
{
    *cnt = 0;
    for (uint64_t d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            fac[(*cnt)++] = d;
            while (n % d == 0) n /= d;
        }
    }
    if (n > 1) fac[(*cnt)++] = n;
}

seq_t a(idx_t n)
{
    /* n is at least 1 per the specification */
    if (n == 1) return (seq_t)1;          /* a(1) = 1 - 0 = 1 */
    uint64_t fac[32];
    size_t cnt;
    factor_distinct_uint64((uint64_t)n, fac, &cnt);

    uint64_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) sum += fac[i];

    return (seq_t)n - (seq_t)sum;
}
