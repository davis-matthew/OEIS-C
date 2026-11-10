/*
 * A084972  When A058033 first reaches n.
 * https://oeis.org/A084972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = least k such that A058033(k) >= n, where A058033(k) =
 *            floor(log2(P_{k+1})) - floor(log2(P_k)) and P_k is the k-th
 *            primorial. Computed by a prime sieve with a running sum of log2
 *            of the primes
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 26 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    unsigned sh = (unsigned)n + 1;
    if (sh < 6) sh = 6;
    if (sh > 40) sh = 40;
    uint64_t limit = (uint64_t)1 << sh;
    uint64_t half = limit / 2;               /* bit i <-> number 2i+1 */
    size_t nbytes = (size_t)(half / 8 + 1);
    unsigned char *comp = (unsigned char *)calloc(nbytes, 1);
    if (!comp) abort();

    for (uint64_t i = 1; ; ++i) {
        uint64_t p = 2 * i + 1;
        if (p * p > limit) break;
        if (comp[i >> 3] & (1u << (i & 7))) continue;
        for (uint64_t j = (p * p - 1) / 2; j < half; j += p)
            comp[j >> 3] |= (unsigned char)(1u << (j & 7));
    }

    long double S = log2l(2.0L);
    long double prevf = floorl(S);
    uint64_t idx = 1;                         /* number of primes summed */
    uint64_t result = 0;
    bool found = false;

    for (uint64_t i = 1; i < half && !found; ++i) {
        if (comp[i >> 3] & (1u << (i & 7))) continue;
        uint64_t p = 2 * i + 1;
        S += log2l((long double)p);
        long double f = floorl(S);
        ++idx;
        /* A058033(idx-1) = f - prevf */
        if ((f - prevf) >= (long double)n) {
            result = idx - 1;
            found = true;
        }
        prevf = f;
    }
    free(comp);
    return (seq_t)result;
}
