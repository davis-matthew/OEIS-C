/*
 * A119450  Primes with odd digit sum.
 * https://oeis.org/A119450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th prime whose decimal digit sum is odd * (OEIS
 *            A119450). The function computes terms on demand and caches *
 *            them so repeated calls are efficient
 * Range:     u8 n <= 28, u16 n <= 3361, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t n) */
#include "../utils/digit_sum.h"  /* seq_t digit_sum(seq_t v) */

static seq_t *cache = NULL;      /* stored sequence values          */
static size_t cache_len = 0;     /* number of terms stored          */
static size_t cache_cap = 0;     /* current array capacity           */
static idx_t next_candidate = 3; /* next odd integer to test        */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n==0, harmless */

    /* gather terms up to n */
    while (cache_len < n) {
        /* search for next prime with odd digit sum */
        for (;;) {
            if (is_prime(next_candidate)) {
                seq_t val = (seq_t)next_candidate;
                if ((digit_sum(val) & 1U) != 0) { /* odd digit sum */
                    /* grow cache if needed */
                    if (cache_len + 1 > cache_cap) {
                        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                        seq_t *tmp = (seq_t *)realloc(cache,
                                                       new_cap * sizeof(seq_t));
                        if (!tmp) abort(); /* out of memory */
                        cache = tmp;
                        cache_cap = new_cap;
                    }
                    cache[cache_len++] = val;    /* store term */
                    next_candidate += 2;         /* advance to next odd */
                    break;
                }
            }
            next_candidate += 2; /* skip even numbers */
        }
    }
    return cache[n - 1];
}
