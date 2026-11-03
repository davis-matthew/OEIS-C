/*
 * A161796  Primes with at least one digit appearing exactly five times in the
 *          decimal expansion.
 * https://oeis.org/A161796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 30 (2026-11-03)
 * Helpers:   digit_frequencies, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: the n‑th prime whose decimal expansion contains a digit
   exactly five times.  Index   a(1)=101111, a(2)=111119, …  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/digit_frequencies.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* External access: a(n) with 1‑based index (a(0) is unused). */
seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)0;
    }

    static seq_t *cache = NULL;   /* array of qualifying primes */
    static size_t  cache_len = 0;
    static size_t  cache_cap = 0;
    static seq_t  next_candidate = 2;   /* candidate to test */

    while (cache_len < n) {
        /* Find the next qualifying prime. */
        while (1) {
            seq_t cand = next_candidate;

            if (is_prime_big(cand)) {
                unsigned int freq[10];
                digit_frequencies(cand, freq);

                bool qualifies = false;
                for (int d = 0; d < 10; ++d) {
                    if (freq[d] == 5) {
                        qualifies = true;
                        break;
                    }
                }
                if (qualifies) {
                    /* Grow the cache if necessary. */
                    if (cache_len == cache_cap) {
                        size_t new_cap = (cache_cap == 0) ? 1 : cache_cap << 1;
                        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                        cache = tmp;
                        cache_cap = new_cap;
                    }
                    cache[cache_len++] = cand;
                    /* Advance to the next integer after the inserted prime. */
                    next_candidate = cand + (seq_t)1;
                    break;
                }
            }

            /* Advance candidate for the next trial. */
            next_candidate = cand + (seq_t)1;
        }
    }

    return cache[n - 1];
}
