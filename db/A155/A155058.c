/*
 * A155058  Primes with odd largest digit.
 * https://oeis.org/A155058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   is_prime_big, odd_largest_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: OEIS A155058 – primes whose largest decimal digit is odd */
/* The code generates the primes on demand; it keeps an internal list
   of already found terms so that subsequent calls are efficient. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"    /* bool is_prime_big(seq_t n) */
#include "../utils/odd_largest_digit.h"

static seq_t *prime_cache = NULL;   /* dynamic array of sequence values */
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Grow the dynamic array to hold at least 'need' elements */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
        prime_cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* n is 1‑based; ignore behaviour for n < 1, as required */
    if (n < (idx_t)1) return (seq_t)0;  /* a safeguard, not used by tests */

    /* Extend the cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        /* Find the next candidate prime */
        uint64_t candidate;
        if (cache_len == 0) {
            candidate = 3;                 /* first prime with odd largest digit */
        } else {
            candidate = (uint64_t)prime_cache[cache_len - 1] + 2;
        }

        /* Advance to the next odd number; skip even candidates */
        for (;;) {
            /* Skip even numbers (except 2 which is not in the list) */
            if (candidate % 2 != 0) {
                seq_t cand_seq = (seq_t)candidate;
                if (is_prime_big(cand_seq) && odd_largest_digit(candidate)) {
                    /* Record the prime */
                    ensure_cache_capacity(cache_len + 1);
                    prime_cache[cache_len++] = cand_seq;
                    break;              /* found next term */
                }
            }
            candidate += 2;               /* next odd number */
        }
    }

    /* Return the requested term */
    return prime_cache[(size_t)n - 1];
}
