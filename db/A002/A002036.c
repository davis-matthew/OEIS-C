/*
 * A002036  Compressed primes: a(n) is the nearest integer to prime(n)/log
 *          prime(n).
 * https://oeis.org/A002036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(prime(n)/log(prime(n))); prime(n) is found with a
 *            sieve of Eratosthenes
 * Range:     u8 n <= 295, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static uint64_t *a002036_primes = NULL;
static uint64_t a002036_count = 0;

static void a002036_sieve(uint64_t need)
{
    uint64_t limit, i, j, cnt;
    unsigned char *comp;
    uint64_t *pr;
    if (need < 6) {
        limit = 20;
    } else {
        long double ln = logl((long double)need);
        limit = (uint64_t)((long double)need * (ln + logl(ln))) + 20;
    }
    for (;;) {
        comp = (unsigned char *)calloc((size_t)(limit + 1), 1);
        cnt = 0;
        for (i = 2; i <= limit; i++) {
            if (!comp[i]) {
                cnt++;
                if (i <= limit / i)
                    for (j = i * i; j <= limit; j += i) comp[j] = 1;
            }
        }
        if (cnt >= need) break;
        free(comp);
        limit *= 2;
    }
    pr = (uint64_t *)malloc((size_t)cnt * sizeof(uint64_t));
    cnt = 0;
    for (i = 2; i <= limit; i++)
        if (!comp[i]) pr[cnt++] = i;
    free(comp);
    free(a002036_primes);
    a002036_primes = pr;
    a002036_count = cnt;
}

seq_t a(idx_t n)
{
    uint64_t p;
    long double r;
    if (n < 1) n = 1;
    if (n > a002036_count) a002036_sieve(n);
    p = a002036_primes[n - 1];
    r = (long double)p / logl((long double)p);
    return (seq_t)(uint64_t)llroundl(r);
}
