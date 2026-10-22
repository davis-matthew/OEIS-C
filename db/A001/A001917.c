/*
 * A001917  a(n) = (p-1)/x, where p = prime(n) and x = ord(2,p), the smallest
 *          positive integer such that 2^x == 1 (mod p).
 * https://oeis.org/A001917
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (p-1)/ord(2,p) with p = prime(n); primes come from a
 *            sieve, ord(2,p) is found by factoring p-1
 * Range:     u8 n <= 1027, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a001917_ord2, a001917_powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a001917_ord2.h"
#include "../utils/a001917_powmod.h"

static uint64_t *a001917_primes = NULL;
static uint64_t a001917_count = 0;

static void a001917_sieve(uint64_t need) {
    uint64_t limit = 1024;
    while (a001917_count < need) {
        char *comp;
        uint64_t i, j, cnt = 0;
        limit *= 2;
        comp = (char *)calloc((size_t)(limit + 1), 1);
        for (i = 2; i * i <= limit; i++)
            if (!comp[i])
                for (j = i * i; j <= limit; j += i) comp[j] = 1;
        for (i = 2; i <= limit; i++) if (!comp[i]) cnt++;
        free(a001917_primes);
        a001917_primes = (uint64_t *)malloc((size_t)cnt * sizeof(uint64_t));
        cnt = 0;
        for (i = 2; i <= limit; i++) if (!comp[i]) a001917_primes[cnt++] = i;
        a001917_count = cnt;
        free(comp);
    }
}

seq_t a(idx_t n) {
    uint64_t p;
    if (n < 2) return (seq_t)0;
    a001917_sieve((uint64_t)n);
    p = a001917_primes[n - 1];
    return (seq_t)((p - 1) / a001917_ord2(p));
}
