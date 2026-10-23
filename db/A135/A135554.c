/*
 * A135554  Numbers k such that k is the fourth power of an integer and the
 *          sum of digits of k is prime.
 * https://oeis.org/A135554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 69, u64 n <= 1090,
 *            u128 n <= 1090, big any n
 * Helpers:   digit_sum, is_prime_small, pow_seq, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A135554:
   numbers k such that k = m^4 for some integer m and the sum of digits of k
   is prime.  The function searches successive fourth powers and stores
   those whose digit sum is prime. */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow_seq.h"
#include "../utils/digit_sum.h"
#include "../utils/is_prime_small.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;      /* array of found terms */
static size_t cache_cap = 0;     /* current allocated capacity */
static size_t cache_size = 0;    /* number of stored terms */
static idx_t next_m = 1;         /* next base to test */

/* Ensure that at least n terms are stored in cache. */
static void ensure_cache(idx_t n)
{
    while (cache_size < (size_t)n) {
        /* Compute m^4 and its digit sum. */
        seq_t k = pow_seq((seq_t)next_m, 4);
        seq_t sum = digit_sum(k);

        /* Store if sum of digits is prime. */
        if (is_prime_small(sum)) {
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_size++] = k;
        }
        ++next_m;
    }
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for offset 1, return 0 */
    ensure_cache(n);
    return cache[n - 1];
}
