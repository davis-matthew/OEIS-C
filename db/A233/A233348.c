/*
 * A233348  Numbers n such that 3*n+2 and 3*n-2 are both prime for n multiple
 *          of 5 (A008587).
 * https://oeis.org/A233348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   is_prime_big, seq_alloc, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A233348
   a(n) = the n-th integer of the form 5*m (m ≥ 1) such that
          3*(5*m) – 2 and 3*(5*m) + 2 are both prime. */

/* External types */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_free.h"

static seq_t *cache      = NULL;   /* array of found terms      */
static size_t cache_size = 0;      /* number of terms stored    */
static size_t cache_cap  = 0;      /* allocated capacity        */
static idx_t  m_counter   = 1;     /* current multiple of 5 to test (5*m_counter) */

static void grow_cache(void)
{
    if (cache_cap < cache_size + 1) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 128;
        seq_t *new_mem = seq_alloc(new_cap);
        if (!new_mem) exit(EXIT_FAILURE);
        for (size_t i = 0; i < cache_size; ++i) new_mem[i] = cache[i];
        seq_free(cache);
        cache = new_mem;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    while (cache_size < n) {
        idx_t candidate = 5 * m_counter;            /* 5*m_counter          */
        seq_t minus     = (seq_t)(3 * candidate - 2);
        seq_t plus      = (seq_t)(3 * candidate + 2);

        if (is_prime_big(minus) && is_prime_big(plus)) {
            grow_cache();
            cache[cache_size] = (seq_t)candidate;
            ++cache_size;
        }
        ++m_counter;
    }
    return cache[n - 1];
}
