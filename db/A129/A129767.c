/*
 * A129767  Interleaved primes and semiprimes.
 * https://oeis.org/A129767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, less, nonn
 * Formula:   a(2*n-1) = A000040(n) (prime), a(2*n) = A001358(n) (semiprime)
 * Range:     u8 n <= 108, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Helper: build a smallest‐prime‑factor array up to a limit */
static void build_spf(idx_t limit, uint32_t **spf_out, size_t *cnt_out) {
    size_t sz = (size_t)limit + 1;
    uint32_t *spf = (uint32_t *)malloc(sz * sizeof(uint32_t));
    if (!spf) exit(1);
    for (size_t i = 0; i < sz; ++i) spf[i] = 0;
    for (uint32_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                /* i is prime */
            spf[i] = i;
            if ((uint64_t)i * i <= limit) {
                for (uint64_t j = (uint64_t)i * i; j <= limit; j += i)
                    if (spf[(size_t)j] == 0) spf[(size_t)j] = i;
            }
        }
    }
    *spf_out = spf;
    *cnt_out = sz;
}

seq_t a(idx_t n) {
    /* static caches */
    static bool ready = false;
    static uint32_t *primes = NULL;
    static size_t primes_cnt = 0;
    static uint32_t *semiprimes = NULL;
    static size_t semiprimes_cnt = 0;

    /* limits adequate for n ≤ 10000 */
    const idx_t PRIME_LIMIT  = 200000;
    const idx_t SEMI_LIMIT   = 200000;

    if (!ready) {
        /* ---- build primes up to PRIME_LIMIT ---- */
        size_t sieve_sz = (size_t)PRIME_LIMIT + 1;
        bool *is_prime = (bool *)malloc(sieve_sz);
        if (!is_prime) exit(1);
        memset(is_prime, true, sieve_sz);
        is_prime[0] = is_prime[1] = false;
        for (uint32_t i = 2; (uint64_t)i * i <= PRIME_LIMIT; ++i)
            if (is_prime[i])
                for (uint64_t j = (uint64_t)i * i; j <= PRIME_LIMIT; j += i)
                    is_prime[(size_t)j] = false;

        /* count primes */
        for (size_t i = 2; i <= PRIME_LIMIT; ++i)
            if (is_prime[i]) ++primes_cnt;

        primes = (uint32_t *)malloc(primes_cnt * sizeof(uint32_t));
        if (!primes) exit(1);
        size_t idx = 0;
        for (size_t i = 2; i <= PRIME_LIMIT; ++i)
            if (is_prime[i]) primes[idx++] = (uint32_t)i;

        free(is_prime);

        /* ---- build semiprimes up to SEMI_LIMIT ---- */
        uint32_t *spf;
        size_t spf_sz;
        build_spf(SEMI_LIMIT, &spf, &spf_sz);

        /* count semiprimes */
        for (size_t num = 4; num <= SEMI_LIMIT; ++num) {
            unsigned cnt = 0;
            size_t x = num;
            while (x > 1 && cnt <= 2) {
                x /= spf[x];
                ++cnt;
            }
            if (cnt == 2) ++semiprimes_cnt;
        }

        semiprimes = (uint32_t *)malloc(semiprimes_cnt * sizeof(uint32_t));
        if (!semiprimes) exit(1);
        idx = 0;
        for (size_t num = 4; num <= SEMI_LIMIT; ++num) {
            unsigned cnt = 0;
            size_t x = num;
            while (x > 1 && cnt <= 2) {
                x /= spf[x];
                ++cnt;
            }
            if (cnt == 2) semiprimes[idx++] = (uint32_t)num;
        }

        free(spf);
        ready = true;
    }

    /* evaluate requested term */
    if (n == 0) return (seq_t)0;        /* offset before 1 is irrelevant */

    if (n & 1) { /* odd index → prime */
        idx_t k = (n + 1) >> 1;         /* k = (n+1)/2 */
        if ((size_t)k-1 >= primes_cnt) return (seq_t)0;
        return (seq_t)primes[(size_t)k - 1];
    } else { /* even index → semiprime */
        idx_t k = n >> 1;               /* k = n/2 */
        if ((size_t)k-1 >= semiprimes_cnt) return (seq_t)0;
        return (seq_t)semiprimes[(size_t)k - 1];
    }
}
