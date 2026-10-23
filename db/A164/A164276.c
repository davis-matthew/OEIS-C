/*
 * A164276  The non-isolated nonprimes.
 * https://oeis.org/A164276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 185, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Non-isolated nonprimes: a(n) is the n-th integer that is not prime and
   has at least one of its immediate neighbours also not prime.
   Indexed from 1 (a(1) = 0). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Return true if n is prime.  n is small (≤ a few hundred thousands),
   so a simple trial division is fast enough. */
static bool is_prime(idx_t n)
{
    if (n <= 1) return false;
    if (n <= 3) return true;
    if ((n & 1) == 0) return false;
    for (idx_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return false;
    return true;
}

/* Return true if n is a non‑prime (≤1 or composite). */
static bool is_nonprime(idx_t n)
{
    return n <= 1 || !is_prime(n);
}

/* Cache of the sequence values.  Initialized lazily. */
static seq_t *cache = NULL;          /* array of sequence values */
static size_t cache_len = 0;         /* number of terms stored */
static size_t cache_cap = 0;         /* allocated capacity */
static idx_t next_candidate = 0;     /* next integer to examine */

/* Ensure the cache contains at least `need` terms. */
static void ensure_cache(idx_t need)
{
    while ((idx_t)cache_len < need) {
        /* Check if next_candidate qualifies as a term. */
        bool left_ok  = false;
        bool right_ok = false;
        bool cur_ok   = is_nonprime(next_candidate);

        if (cur_ok) {
            if (next_candidate > 0)
                left_ok = is_nonprime(next_candidate - 1);
            right_ok = is_nonprime(next_candidate + 1);
        }

        if (cur_ok && (left_ok || right_ok)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);  /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        next_candidate++;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for n < 1; return 0 */
    ensure_cache(n);
    return cache[n - 1];
}
