/*
 * A213973  List of imprimitive words over the alphabet {1,3}.
 * https://oeis.org/A213973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th lexicographically‑ordered imprimitive word over
 *            the alphabet {1,3}. An imprimitive word is one that can be
 *            written as u^k with k>1
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   is_imprimitive
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_imprimitive.h"

/* convert a binary word of length len to its decimal representation
   over the alphabet {1,3} (most significant bit determines the
   first digit). */
static inline seq_t bits_to_value(uint64_t bits, idx_t len)
{
    seq_t val = (seq_t)0;
    for (idx_t i = 0; i < len; ++i) {
        uint64_t digit_bit = (bits >> (len - 1 - i)) & 1ULL;
        val = val * (seq_t)10 + ((digit_bit == 0ULL) ? (seq_t)1 : (seq_t)3);
    }
    return val;
}

/* cache of already generated terms */
static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* number of elements stored */
static idx_t cache_cap  = 0;   /* allocated capacity */

/* next length and next binary number to test */
static idx_t gen_next_len   = 2;   /* start with length 2 (the first word) */
static uint64_t gen_next_num = 0;

/* allocate or grow the cache to fit at least 'needed' entries */
static void ensure_cache_cap(idx_t needed)
{
    if (needed <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 1024;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* generate terms up to (and including) index 'n' (1‑based). */
static void ensure_up_to(idx_t n)
{
    while (cache_size < n) {
        uint64_t limit = ((uint64_t)1) << gen_next_len;
        for (; gen_next_num < limit; ++gen_next_num) {
            if (is_imprimitive(gen_next_num, gen_next_len)) {
                seq_t val = bits_to_value(gen_next_num, gen_next_len);
                ensure_cache_cap(cache_size + 1);
                cache[cache_size++] = val;
                if (cache_size == n) {
                    /* Advance state so that next call continues after
                       this word. */
                    ++gen_next_num;
                    return;
                }
            }
        }
        /* finished this length, move to the next */
        ++gen_next_len;
        gen_next_num = 0;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* sequence is defined for n ≥ 1 */
    ensure_up_to(n);
    return cache[n - 1];
}
