/*
 * A215321  Primes congruent to {1, 2, 3, 5, 6} mod 19.
 * https://oeis.org/A215321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime ≡ 1,2,3,5,6 (mod 19) (search approach)
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Cache of all primes found so far (uint64_t) */
static uint64_t *all_primes = NULL;
static size_t   all_count  = 0;
static size_t   all_cap    = 0;

/* Cache of the required primes (seq_t) */
static seq_t *allowed = NULL;
static size_t allowed_count = 0;
static size_t allowed_cap   = 0;

/* The largest integer tested for primality */
static uint64_t last_checked = 1;

/* Append a new prime to the all_primes array, expanding capacity if needed */
static void push_all_prime(uint64_t p)
{
    if (all_count >= all_cap) {
        size_t new_cap = all_cap == 0 ? 4 : all_cap * 2;
        uint64_t *tmp = (uint64_t *)realloc(all_primes, new_cap * sizeof(uint64_t));
        if (!tmp) exit(1);            /* fatal error */
        all_primes = tmp;
        all_cap = new_cap;
    }
    all_primes[all_count++] = p;
}

/* Append a new required prime (as seq_t) to the allowed array */
static void push_allowed(seq_t p)
{
    if (allowed_count >= allowed_cap) {
        size_t new_cap = allowed_cap == 0 ? 4 : allowed_cap * 2;
        seq_t *tmp = (seq_t *)realloc(allowed, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);            /* fatal error */
        allowed = tmp;
        allowed_cap = new_cap;
    }
    allowed[allowed_count++] = p;
}

/* Generate primes until the allowed array contains at least `need` terms */
static void ensure_cache(idx_t need)
{
    if (allowed_count >= need) return;

    for (;;) {
        ++last_checked;
        if (last_checked < 2) continue;        /* skip 0,1 */

        bool is_prime = true;
        for (size_t i = 0; i < all_count; ++i) {
            uint64_t p = all_primes[i];
            if (p * p > last_checked) break;
            if (last_checked % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            push_all_prime(last_checked);
            uint64_t rem = last_checked % 19ULL;
            if (rem == 1 || rem == 2 || rem == 3 || rem == 5 || rem == 6)
                push_allowed((seq_t)last_checked);
        }
        if (allowed_count >= need) break;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined for n < 1 */
    ensure_cache((size_t)n);
    return allowed[n - 1];
}
