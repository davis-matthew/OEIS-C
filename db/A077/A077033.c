/*
 * A077033  Numbers k such that A068340(k) = +/-5.
 * https://oeis.org/A077033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, hard
 * Formula:   a(n) = n-th k with A068340(k) = Sum_{j=1..k} mu(j)*j equal to +5
 *            or -5; found by a sieve search (only 4 terms are known below
 *            10^12)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 4 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define LIM_A077033 20000

static int A077033_ready = 0;
static int A077033_cnt = 0;
static uint32_t A077033_terms[64];

static void A077033_init(void)
{
    int8_t mu[LIM_A077033 + 1];
    uint8_t comp[LIM_A077033 + 1];
    int primes[LIM_A077033];
    int np = 0;
    int i, j;
    long long s = 0;

    for (i = 0; i <= LIM_A077033; i++) { comp[i] = 0; mu[i] = 0; }
    mu[1] = 1;
    for (i = 2; i <= LIM_A077033; i++) {
        if (!comp[i]) { primes[np++] = i; mu[i] = -1; }
        for (j = 0; j < np && (long long)i * primes[j] <= LIM_A077033; j++) {
            int m = i * primes[j];
            comp[m] = 1;
            if (i % primes[j] == 0) { mu[m] = 0; break; }
            mu[m] = (int8_t)(-mu[i]);
        }
    }
    for (i = 1; i <= LIM_A077033; i++) {
        s += (long long)mu[i] * i;
        if ((s == 5 || s == -5) && A077033_cnt < 64)
            A077033_terms[A077033_cnt++] = (uint32_t)i;
    }
    A077033_ready = 1;
}

seq_t a(idx_t n)
{
    if (!A077033_ready)
        A077033_init();
    if (n < 1 || n > (idx_t)A077033_cnt)
        return (seq_t)0;
    return (seq_t)A077033_terms[n - 1];
}
