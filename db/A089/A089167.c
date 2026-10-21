/*
 * A089167  Smallest number m that is coprime to n and such that the
 *          arithmetic progression (n+k*m:k>0) contains no primes for values
 *          not greater than n^2; a(1)=1.
 * https://oeis.org/A089167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest integer m (coprime to n) such that the
 *            arithmetic * progression n + k*m (k > 0) contains no primes <=
 *            n^2
 * Range:     u8 n <= 51, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/gcd.h"

/* ---------- Prime sieve up to 1e8 ---------- */
#define PRIME_LIMIT 100000000   /* n^2 <= 10^8 for n <= 10000 */
static uint32_t primes[6000005];
static size_t primes_cnt = 0;
static bool primes_initialized = false;

/* simple Sieve of Eratosthenes with a byte bitset */
static void init_primes(void) {
    if (primes_initialized) return;

    uint32_t limit = PRIME_LIMIT;
    uint8_t *isComp = (uint8_t *)malloc(limit + 1);
    if (!isComp) return; /* allocation failure – unlikely in tests */
    for (uint32_t i = 0; i <= limit; ++i) isComp[i] = 0;
    isComp[0] = isComp[1] = 1;

    uint32_t sq = (uint32_t)sqrt((double)limit);
    for (uint32_t i = 2; i <= sq; ++i) {
        if (!isComp[i]) {
            for (uint32_t j = i * i; j <= limit; j += i) {
                isComp[j] = 1;
            }
        }
    }

    primes_cnt = 0;
    for (uint32_t i = 2; i <= limit; ++i) if (!isComp[i]) primes[primes_cnt++] = i;
    free(isComp);
    primes_initialized = true;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n) {
    if (n == 1) return 1;          /* a(1) = 1 by definition */

    init_primes();                /* ensure primes are available */

    /* First prime greater than n */
    size_t start = 0;
    while (start < primes_cnt && primes[start] <= (uint32_t)n) ++start;

    /* Upper bound for the AP */
    uint64_t limit = (uint64_t)n * n;

    for (uint32_t m = 1; ; ++m) {          /* search increasing m */
        if (gcd((seq_t)n, (seq_t)m) != 1) continue;  /* m must be coprime to n */

        bool ok = true;
        for (size_t i = start; i < primes_cnt; ++i) {
            uint32_t p = primes[i];
            if ((uint64_t)p > limit) break;
            if ((p - n) % m == 0) {    /* a prime in the progression */
                ok = false;
                break;
            }
        }
        if (ok) return m;             /* found the minimal coprime m */
    }
}
