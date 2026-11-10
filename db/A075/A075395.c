/*
 * A075395  Composite members of A075394.
 * https://oeis.org/A075395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th composite k such that S(k-1) < S(k) > S(k+1), S =
 *            Kempner function A002034 (sieve search)
 * Range:     u8 n <= 35, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A075395_minm(uint64_t p, uint64_t e)
{
    uint64_t low = e, high = e * p;
    while (low < high) {
        uint64_t mid = (low + high) >> 1;
        uint64_t ex = 0, t = mid;
        while (t) { t /= p; ex += t; }
        if (ex >= e) high = mid; else low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    static uint64_t *terms = NULL;
    static size_t cnt = 0;
    static uint64_t limit = 500;
    static bool done = false;

    while (!done || cnt < (size_t)n) {
        if (done) limit *= 2;
        done = true;
        size_t L = (size_t)limit;
        uint32_t *spf = (uint32_t *)calloc(L + 2, sizeof(uint32_t));
        uint32_t *S = (uint32_t *)calloc(L + 2, sizeof(uint32_t));
        for (size_t i = 2; i <= L + 1; ++i) {
            if (spf[i] == 0) {
                for (size_t j = i; j <= L + 1; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
        for (size_t i = 2; i <= L + 1; ++i) {
            uint64_t x = i, best = 0;
            while (x > 1) {
                uint64_t p = spf[x];
                uint64_t c = 0;
                while (x % p == 0) { x /= p; ++c; }
                uint64_t m = (c == 1) ? p : A075395_minm(p, c);
                if (m > best) best = m;
            }
            S[i] = (uint32_t)best;
        }
        free(terms);
        terms = (uint64_t *)malloc((L / 2 + 16) * sizeof(uint64_t));
        cnt = 0;
        for (size_t i = 4; i <= L; ++i) {
            if (spf[i] != i && S[i - 1] < S[i] && S[i] > S[i + 1])
                terms[cnt++] = i;
        }
        free(spf);
        free(S);
    }
    return (seq_t)terms[(size_t)n - 1];
}
