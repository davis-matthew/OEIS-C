/*
 * A153143  Nonnegative numbers k such that 2k + 19 is prime.
 * https://oeis.org/A153143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (A000040(n+7) - 19)/2 (the (n+6)-th prime minus 19,
 *            divided by 2)
 * Range:     u8 n <= 47, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static uint64_t *primes = NULL;
static size_t primes_size = 0;
static size_t primes_cap = 0;
static uint64_t next_candidate = 2;

/* Ensure that primes[0..need_index] are stored.  */
static void ensure_primes(size_t need_index)
{
    if (primes == NULL) {
        primes_cap = 16;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        primes_size = 0;
        next_candidate = 2;
    }

    while (primes_size <= need_index) {
        uint64_t cand = next_candidate;

        /* Test cand for primality using previously found primes. */
        int is_prime = 1;
        for (size_t i = 0; i < primes_size; ++i) {
            uint64_t p = primes[i];
            unsigned __int128 pp = (unsigned __int128)p * p;
            if (pp > cand) break;
            if (cand % p == 0) { is_prime = 0; break; }
        }

        if (is_prime) {
            if (primes_size >= primes_cap) {
                primes_cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                if (!tmp) exit(EXIT_FAILURE);
                primes = tmp;
            }
            primes[primes_size++] = cand;
        }

        /* Advance candidate to next potential odd number. */
        if (cand == 2) next_candidate = 3;
        else next_candidate += 2;
    }
}

seq_t a(idx_t n)
{
    /* Need the (n+6)-th prime (0‑based index = n+6). */
    size_t need = (size_t)(n + 6);
    ensure_primes(need);

    seq_t prime = (seq_t)primes[need];
    seq_t k = (prime - (seq_t)19) / 2;
    return k;
}
