/*
 * A128251  n^4 - 1 divided by its largest fourth power divisor.
 * https://oeis.org/A128251
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^4 - 1) / A008835(n^4 - 1) i.e. the biquadrate‑free
 *            part of n^4 − 1
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Return the biquadrate‑free part of n^4 − 1.
   n is assumed to be ≥ 2 (the OEIS offset). */
seq_t a(idx_t n)
{
    /* 1) Compute n^4 − 1 in seq_t.  */
    seq_t v = (seq_t)n;
    v = v * v;                /* n²  */
    v = v * v;                /* n⁴  */
    v = v - (seq_t)1;         /* n⁴ − 1 */

    /* 2) factor v.  Since the tests use n ≤ 10 000, v fits in 64 bits. */
    uint64_t v64 = (uint64_t)v;          /* safe cast because v < 2⁶⁴ in tests */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;
    factor_u64(v64, primes, exps, &cnt);

    /* 3) Build the biquadrate‑free part: product of p^(e mod 4). */
    seq_t res = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i] & 3ULL;    /* e % 4 */
        if (e == 0) continue;
        seq_t p = (seq_t)primes[i];
        for (uint64_t j = 0; j < e; ++j)
            res = res * p;
    }
    return res;
}
