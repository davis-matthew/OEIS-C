/*
 * A049078  Primes prime(k) for which A049076(k) = 2.
 * https://oeis.org/A049078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(A007821(n)) a(n) is the n-th prime whose index k =
 *            π(p) is itself prime, but the index of k, namely π(k), is not
 *            prime. The implementation precomputes all such primes up to 5
 *            000 000, which yields more than 20 000 terms – enough for the
 *            required n ≤ 10 000. All arithmetic stays well...
 * Range:     u8 n <= 10, u16 n <= 698, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_or_die
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_or_die.h"

seq_t a(idx_t n) {
    /* static data shared among all calls */
    static bool initialized = false;
    static uint8_t *is_prime = NULL;     /* primality bitset up to LIMIT */
    static int *pi_cnt = NULL;           /* prefix prime counts up to LIMIT */
    static uint32_t *primes = NULL;      /* list of all primes ≤ LIMIT */
    static seq_t *seq = NULL;            /* sequence a(n) */
    static size_t seq_cnt = 0;           /* number of computed terms */

    /* constants */
    const size_t LIMIT = 5000000;        /* sieve limit (≥ needed for n≈10 000) */
    const size_t SEQ_CAP_INIT = 40000;   /* initial capacity of seq array */

    if (!initialized) {
        initialized = true;

        /* 1. Sieve of Eratosthenes up to LIMIT */
        is_prime = (uint8_t *)alloc_or_die(LIMIT + 1, sizeof(uint8_t));
        for (size_t i = 0; i <= LIMIT; ++i) is_prime[i] = 1;
        is_prime[0] = is_prime[1] = 0;
        for (size_t i = 2; i * i <= LIMIT; ++i) {
            if (is_prime[i]) {
                for (size_t j = i * i; j <= LIMIT; j += i)
                    is_prime[j] = 0;
            }
        }

        /* 2. Prefix counts π(x) for 0 ≤ x ≤ LIMIT */
        pi_cnt = (int *)alloc_or_die(LIMIT + 1, sizeof(int));
        int cnt = 0;
        for (size_t i = 0; i <= LIMIT; ++i) {
            if (i >= 2 && is_prime[i]) ++cnt;
            pi_cnt[i] = cnt;
        }

        /* 3. Build list of all primes ≤ LIMIT */
        size_t prime_count = 0;
        for (size_t i = 2; i <= LIMIT; ++i)
            if (is_prime[i]) ++prime_count;
        primes = (uint32_t *)alloc_or_die(prime_count, sizeof(uint32_t));
        size_t p_idx = 0;
        for (size_t i = 2; i <= LIMIT; ++i)
            if (is_prime[i]) primes[p_idx++] = (uint32_t)i;

        /* 4. Build the sequence a(n) */
        seq = (seq_t *)alloc_or_die(SEQ_CAP_INIT, sizeof(seq_t));
        seq_cnt = 0;
        size_t seq_cap = SEQ_CAP_INIT;

        for (size_t i = 0; i < prime_count; ++i) {
            uint32_t p  = primes[i];
            size_t k    = i + 1;          /* π(p), 1‑based index of this prime */

            /* Need k to be prime, and π(k) to be composite */
            if (k <= LIMIT && is_prime[k]) {      /* k itself is prime */
                int pi_k = pi_cnt[k];             /* π(k) */
                bool pi_k_prime = (pi_k >= 2) && (pi_k <= (int)LIMIT) && is_prime[pi_k];
                if (!pi_k_prime) {                /* π(k) not prime */
                    if (seq_cnt == seq_cap) {     /* grow seq array */
                        seq_cap *= 2;
                        seq = (seq_t *)realloc(seq,
                            seq_cap * sizeof(seq_t));
                        if (!seq) exit(EXIT_FAILURE);
                    }
                    seq[seq_cnt++] = (seq_t)p;   /* store prime as seq_t */
                }
            }
        }
    }

    /* Return the required term; return 0 if n is out of range. */
    if (n == 0 || (idx_t)seq_cnt < n)
        return (seq_t)0;
    return seq[n - 1];
}
