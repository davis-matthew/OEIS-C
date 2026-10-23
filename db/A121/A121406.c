/*
 * A121406  a(1) = a(2) = 0; a(3) = 2; for n >= 4, a(n) =
 *          (prime(n-1)-2)*a(n-1), where prime(n) is the n-th prime.
 * https://oeis.org/A121406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=a(2)=0; a(3)=2; for n>=4, a(n)=(prime(n-1)-2)*a(n-1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 11, u64 n <= 17, u128 n <= 27,
 *            big any n
 * Helpers:   grow_prime_cap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_prime_cap.h"

/* Helper to grow a zero‑initialised array of seq_t */
static void grow_seq_cache(idx_t *cap, seq_t **ptr, idx_t needed)
{
    if (*cap >= needed) return;
    idx_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    *ptr = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
    if (*ptr == NULL) abort();
    *cap = new_cap;
}

/* Test primality of a candidate using already generated primes. */
/* primes[], primes_len must contain all primes < candidate. */
static bool is_prime_candidate(uint32_t candidate, uint32_t *primes, size_t primes_len)
{
    if (candidate < 2) return false;
    if (candidate % 2 == 0) return candidate == 2;
    uint64_t limit = (uint64_t)candidate;
    for (size_t i = 0; i < primes_len && (uint64_t)primes[i] * primes[i] <= limit; ++i) {
        if (candidate % primes[i] == 0) return false;
    }
    return true;
}

/* Compute a(n) according to the recurrence. */
seq_t a(idx_t n)
{
    /* static cache for the sequence */
    static seq_t *seq_cache = NULL;
    static idx_t seq_cap = 0;
    static idx_t seq_len = 0;          /* maximum n that has been computed */

    /* static cache for primes */
    static uint32_t *primes = NULL;
    static size_t prime_cap = 0;
    static size_t prime_len = 0;       /* number of generated primes */

    /* initialise caches (first call) */
    if (seq_len == 0) {
        /* initialise prime list with first three primes: 2,3,5 */
        grow_prime_cap(&prime_cap, &primes, 3);
        primes[0] = 2; primes[1] = 3; primes[2] = 5;
        prime_len = 3;

        /* initialise sequence cache with a(1)=0, a(2)=0, a(3)=2 */
        grow_seq_cache(&seq_cap, &seq_cache, 4);   /* indices 0..3 used, 0 unused */
        seq_cache[1] = 0;
        seq_cache[2] = 0;
        seq_cache[3] = 2;
        seq_len = 3;
    }

    /* if requested value is already cached, return it */
    if (n <= seq_len) return seq_cache[n];

    /* ensure primes list has at least prime(n-1) (i.e. prime index n-1) */
    while (prime_len < (size_t)(n - 1)) {
        uint32_t candidate = primes[prime_len - 1] + 2;   /* skip even numbers */
        while (!is_prime_candidate(candidate, primes, prime_len)) {
            candidate += 2;
        }
        grow_prime_cap(&prime_cap, &primes, prime_len + 1);
        primes[prime_len++] = candidate;
    }

    /* grow sequence cache up to n */
    grow_seq_cache(&seq_cap, &seq_cache, n + 1);

    /* compute successive terms until we reach n */
    for (idx_t i = seq_len + 1; i <= n; ++i) {
        seq_t factor = (seq_t)(primes[(size_t)(i - 2)] - 2); /* prime(i-1)-2 */
        seq_cache[i] = seq_cache[i - 1] * factor;
    }
    seq_len = n;
    return seq_cache[n];
}
