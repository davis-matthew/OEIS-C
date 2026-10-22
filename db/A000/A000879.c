/*
 * A000879  Number of primes < prime(n)^2.
 * https://oeis.org/A000879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000720(prime(n)^2); prime(n) comes from a sieve, and
 *            pi(x) is computed with the Lucy_Hedgehog prime-counting
 *            algorithm
 * Range:     u8 n <= 12, u16 n <= 154, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   prime_pi_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/prime_pi_square.h"

static uint64_t *primes = NULL;
static uint64_t nprimes = 0;
static uint64_t sieved_to = 0;

static void ensure_primes(uint64_t n) {
    uint64_t L = sieved_to ? sieved_to : 1024;
    while (nprimes < n) {
        L *= 2;
        unsigned char *s = (unsigned char *)malloc(L + 1);
        memset(s, 1, L + 1);
        s[0] = s[1] = 0;
        for (uint64_t i = 2; i * i <= L; i++)
            if (s[i])
                for (uint64_t j = i * i; j <= L; j += i) s[j] = 0;
        uint64_t cnt = 0;
        for (uint64_t i = 2; i <= L; i++) if (s[i]) cnt++;
        free(primes);
        primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
        nprimes = 0;
        for (uint64_t i = 2; i <= L; i++) if (s[i]) primes[nprimes++] = i;
        free(s);
        sieved_to = L;
    }
}

seq_t a(idx_t n) {
    ensure_primes((uint64_t)n);
    uint64_t p = primes[n - 1];
    seq_t result = (seq_t)prime_pi_square(p);
    return result;
}
