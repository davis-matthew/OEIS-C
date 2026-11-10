/*
 * A239282  a(n) = A045917(n)*prime(n).
 * https://oeis.org/A239282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A045917(n)*prime(n); computed with a sieve of
 *            Eratosthenes (Goldbach pair count of 2n times the n-th prime)
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static unsigned char *A239282_sv = NULL;
static idx_t A239282_lim = 0;

static void A239282_ensure(idx_t limit)
{
    if (limit <= A239282_lim)
        return;
    unsigned char *s = (unsigned char *)malloc((size_t)(limit + 1));
    if (!s)
        exit(EXIT_FAILURE);
    for (idx_t i = 0; i <= limit; ++i)
        s[i] = (i >= 2);
    for (idx_t i = 2; i * i <= limit; ++i)
        if (s[i])
            for (idx_t j = i * i; j <= limit; j += i)
                s[j] = 0;
    free(A239282_sv);
    A239282_sv = s;
    A239282_lim = limit;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t lim = 2 * n + 20;
    idx_t pn = 0;
    for (;;) {
        A239282_ensure(lim);
        idx_t cnt = 0;
        for (idx_t i = 2; i <= A239282_lim; ++i) {
            if (A239282_sv[i]) {
                ++cnt;
                if (cnt == n) {
                    pn = i;
                    break;
                }
            }
        }
        if (pn)
            break;
        lim = A239282_lim * 2;
    }
    idx_t g = 0;
    for (idx_t p = 2; p <= n; ++p)
        if (A239282_sv[p] && A239282_sv[2 * n - p])
            ++g;
    return (seq_t)g * (seq_t)pn;
}
