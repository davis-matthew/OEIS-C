/*
 * A072206  Third terms of triple Peano sequence A071988.
 * https://oeis.org/A072206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000125(prime(n)-1) = (m^3+5*m+6)/6 with m = prime(n)-1
 *            (primes by sieve)
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 425, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 53 terms through n = 9554 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *a072206_primes = NULL;
static uint64_t a072206_np = 0;
static uint64_t a072206_lim = 0;

static void a072206_build(uint64_t lim)
{
    unsigned char *comp = (unsigned char *)malloc((size_t)(lim + 1));
    uint64_t i, j, cnt = 0;
    memset(comp, 0, (size_t)(lim + 1));
    for (i = 2; i * i <= lim; i++)
        if (!comp[i])
            for (j = i * i; j <= lim; j += i)
                comp[j] = 1;
    for (i = 2; i <= lim; i++)
        if (!comp[i])
            cnt++;
    free(a072206_primes);
    a072206_primes = (uint64_t *)malloc((size_t)(cnt + 1) * sizeof(uint64_t));
    cnt = 0;
    for (i = 2; i <= lim; i++)
        if (!comp[i])
            a072206_primes[cnt++] = i;
    free(comp);
    a072206_np = cnt;
    a072206_lim = lim;
}

static seq_t a072206_cake(idx_t n)
{
    idx_t x, y, z;
    seq_t r;
    if (n < 2)
        return (seq_t)n + 1;
    x = n + 1;
    y = n;
    z = n - 1;
    if (x % 2 == 0) x /= 2; else y /= 2;
    if (x % 3 == 0) x /= 3;
    else if (y % 3 == 0) y /= 3;
    else z /= 3;
    r = (seq_t)x;
    r *= (seq_t)y;
    r *= (seq_t)z;
    r += (seq_t)n;
    r += 1;
    return r;
}

seq_t a(idx_t n)
{
    uint64_t lim = a072206_lim ? a072206_lim : 64;
    while (a072206_np < n) {
        if (a072206_lim >= lim)
            lim *= 2;
        a072206_build(lim);
    }
    return a072206_cake((idx_t)(a072206_primes[n - 1] - 1));
}
