/*
 * A078570  Number of distinct prime factors of the average of n-th twin prime
 *          pair.
 * https://oeis.org/A078570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001221(A014574(n)) Number of distinct prime factors of
 *            the average of the n-th twin prime pair. The average of the twin
 *            pair (p, p+2) is (p+1). Thus a(n) = ω(p+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static void init_sequence(void);

/* Count distinct prime factors of a number ≤ 2,000,002 using a pre‑computed SPF array. */
static seq_t omega_of(uint32_t m);

/* Pre‑computed data */
static uint32_t *spf = NULL;          /* smallest prime factor for each number */
static seq_t   *omega = NULL;         /* omega values for twin primes */
static size_t   omega_len = 0;        /* number of computed twin primes */
static const size_t LIMIT = 2000000;  /* sufficient for n up to at least 10 000 */

static void init_sequence(void)
{
    if (omega != NULL) return; /* already initialized */

    /* Allocate and compute SPF up to LIMIT+1 */
    size_t sz = LIMIT + 2;   /* include LIMIT+1 */
    spf = (uint32_t*)malloc(sz * sizeof(uint32_t));
    for (size_t i = 0; i < sz; ++i) spf[i] = 0;
    spf[0] = spf[1] = 1;
    for (size_t i = 2; i * i <= LIMIT + 1; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            for (size_t j = i * i; j <= LIMIT + 1; j += i) {
                if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
    for (size_t i = 2; i <= LIMIT + 1; ++i) {
        if (spf[i] == 0) spf[i] = (uint32_t)i;
    }

    /* Build list of primes up to LIMIT */
    size_t prime_capacity = 200000;  /* initial capacity */
    uint32_t *primes = (uint32_t*)malloc(prime_capacity * sizeof(uint32_t));
    size_t prime_cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == (uint32_t)i) {
            if (prime_cnt == prime_capacity) {
                prime_capacity *= 2;
                primes = (uint32_t*)realloc(primes, prime_capacity * sizeof(uint32_t));
            }
            primes[prime_cnt++] = (uint32_t)i;
        }
    }

    /* Scan twin primes and compute omega(p+1) */
    /* We store omega values in an array of seq_t. */
    size_t twin_capacity = prime_cnt / 10;  /* rough estimate */
    omega = (seq_t*)malloc(twin_capacity * sizeof(seq_t));
    omega_len = 0;
    for (size_t i = 0; i + 1 < prime_cnt; ++i) {
        if (primes[i+1] == primes[i] + 2) {
            uint32_t p = primes[i];
            seq_t val = omega_of(p + 1);
            if (omega_len == twin_capacity) {
                twin_capacity *= 2;
                omega = (seq_t*)realloc(omega, twin_capacity * sizeof(seq_t));
            }
            omega[omega_len++] = val;
        }
    }

    free(primes);
    /* spf can be freed as it's no longer needed */
    free(spf);
    spf = NULL;
}

static seq_t omega_of(uint32_t m)
{
    seq_t cnt = 0;
    uint32_t prev = 0;
    while (m > 1) {
        uint32_t p = spf[m];
        if (p != prev) {
            ++cnt;
            prev = p;
        }
        while (m % p == 0) m /= p;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1 */
    init_sequence();
    if ((size_t)n > omega_len) return 0; /* safety */
    return omega[(size_t)n - 1];
}
