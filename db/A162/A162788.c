/*
 * A162788  a(n) = A162528(n)/8.
 * https://oeis.org/A162788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A162528(n)/8. For n ≥ 9, a(n) = prime(n‑3); For n ≤ 8,
 *            the values are 8, 9, 10, 11, 12, 13, 14, 16 respectively
 * Range:     u8 n <= 56, u16 n <= 6544, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Prime cache in 64‑bit integer form.  Seq values are cast on return. */
static uint64_t *prime_cache = NULL;
static size_t    prime_cnt   = 0;
static size_t    prime_cap   = 0;
static uint64_t  next_candidate = 0;

/* Grow the prime cache to hold at least `need` elements. */
static void grow_prime_cache(size_t need)
{
    if (prime_cap < need) {
        size_t new_cap = prime_cap == 0 ? 16 : prime_cap << 1;
        while (new_cap < need) new_cap <<= 1;
#if defined(__cplusplus)
        uint64_t *tmp = static_cast<uint64_t *>(realloc(prime_cache,
                                                        new_cap * sizeof(uint64_t)));
#else
        uint64_t *tmp = (uint64_t *)realloc(prime_cache,
                                            new_cap * sizeof(uint64_t));
#endif
        prime_cache = tmp;
        prime_cap   = new_cap;
    }
}

/* Check whether x is prime using the current cache of smaller primes. */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_cache[i];
        /* Avoid overflow in p*p by using division. */
        if (p > x / p) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* Ensure that prime_cache contains primes[0..idx] inclusive. */
static void ensure_primes_up_to_index(size_t idx)
{
    /* Initialise with the first prime 2 if necessary. */
    if (prime_cnt == 0) {
        grow_prime_cache(1);
        prime_cache[0] = 2;
        prime_cnt = 1;
        next_candidate = 3;
    }

    while (prime_cnt <= idx) {
        uint64_t cand = next_candidate;
        /* Skip even numbers after 2. */
        while (true) {
            if (is_prime_uint64(cand)) {
                grow_prime_cache(prime_cnt + 1);
                prime_cache[prime_cnt++] = cand;
                next_candidate = cand + 2;
                break;
            }
            cand += 2;
            next_candidate = cand;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for n < 1 */

    /* The first eight terms are special. */
    if (n <= 8) {
        switch ((int)n) {
            case 1: return (seq_t)8;
            case 2: return (seq_t)9;
            case 3: return (seq_t)10;
            case 4: return (seq_t)11;
            case 5: return (seq_t)12;
            case 6: return (seq_t)13;
            case 7: return (seq_t)14;
            case 8: return (seq_t)16;
            default: return 0;  /* never reached */
        }
    }

    /* For n ≥ 9: a(n) = prime(n‑3) (0‑based indexing). */
    size_t idx = (size_t)(n - 3);
    ensure_primes_up_to_index(idx);
    return (seq_t)prime_cache[idx];
}
