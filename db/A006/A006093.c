/*
 * A006093  a(n) = prime(n) - 1.
 * https://oeis.org/A006093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = prime(n) - 1; primes found by a sieve of Eratosthenes
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *a006093_primes = NULL;
static uint64_t a006093_np = 0;
static uint64_t a006093_lim = 0;

static void a006093_build(uint64_t lim)
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
    free(a006093_primes);
    a006093_primes = (uint64_t *)malloc((size_t)(cnt + 1) * sizeof(uint64_t));
    cnt = 0;
    for (i = 2; i <= lim; i++)
        if (!comp[i])
            a006093_primes[cnt++] = i;
    free(comp);
    a006093_np = cnt;
    a006093_lim = lim;
}

seq_t a(idx_t n)
{
    uint64_t lim = a006093_lim ? a006093_lim : 64;
    while (a006093_np < n) {
        if (a006093_lim >= lim)
            lim *= 2;
        a006093_build(lim);
    }
    return (seq_t)(a006093_primes[n - 1] - 1);
}
