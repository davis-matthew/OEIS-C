/*
 * A112998  Numbers n such that n, n+1 and n+2 are 1,2,3-almost primes.
 * https://oeis.org/A112998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer n such that * n is prime, * n+1 is a
 *            semiprime, * n+2 is a 3‑almost prime (Ω(n+2)=3). The
 *            implementation performs an incremental search over all n ≡ 1
 *            (mod 12) and stores the found terms in a cache to satisfy
 *            out‑of‑order requests
 * Range:     u8 n <= 3, u16 n <= 176, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bigomega_uint64, ensure_capacity, is_prime, is_semiprime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/is_semiprime_uint64.h"
#include "../utils/bigomega_uint64.h"
#include "../utils/ensure_capacity.h"

/* Cache of already found terms */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;   /* allocated capacity */
static idx_t  cache_size = 0;   /* number of terms stored */

/* The next candidate value (≡ 1 mod 12) to test */
static uint64_t next_candidate = 1;

/* Return the nth term (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1 */

    /* Extend the cache until it contains at least n terms. */
    while (cache_size < n) {
        uint64_t cand = next_candidate;          /* candidate n */
        if (is_prime(cand)) {
            uint64_t nm1 = cand + 1;
            uint64_t nm2 = cand + 2;
            if (is_semiprime_uint64(nm1) &&
                bigomega_uint64(nm2) == (seq_t)3) {
                /* Ensure the array is large enough. */
                if (cache_size + 1 > cache_cap)
                    ensure_capacity(cache_size + 1, &cache, &cache_cap);
                /* Store the found term. */
                cache[cache_size++] = (seq_t)cand;
            }
        }
        /* Move to the next number ≡ 1 mod 12. */
        next_candidate += 12;
    }

    return cache[n - 1];
}
