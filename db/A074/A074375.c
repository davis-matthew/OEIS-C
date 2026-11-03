/*
 * A074375  s(s+3)/2 where s is the sum of the prime factors of n (with
 *          repetition).
 * https://oeis.org/A074375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = s*(s+3)/2 where s is the sum of the prime factors of n
 *            (with multiplicity)
 * Range:     u8 n <= 22, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-01)
 * Helpers:   factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_uint64.h"

seq_t a(idx_t n)
{
    /* Sum of prime factors with multiplicity */
    uint64_t fac[20];
    int exp[20];
    int cnt = 0;
    factor_uint64(n, fac, exp, &cnt);

    seq_t s = 0;
    for (int i = 0; i < cnt; ++i) {
        s += (seq_t)fac[i] * (seq_t)exp[i];
    }

    /* Compute s*(s+3)/2 using 128‑bit intermediate to avoid overflow. */
    unsigned __int128 prod = (unsigned __int128)s * ((unsigned __int128)s + 3ULL);
    return (seq_t)(prod / 2ULL);
}
