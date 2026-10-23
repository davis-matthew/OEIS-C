/*
 * A125292  Numbers having either no ones or no twos in their ternary
 *          representation.
 * https://oeis.org/A125292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth smallest positive integer whose ternary
 *            representation uses only digits 0/1 or only digits 0/2. Sequence
 *            consists of sorting the union of A005823 (digits 0/1) and
 *            A005836 (digits 0/2)
 * Range:     u8 n <= 62, u16 n <= 2301, u32 n <= 8190, u64 n <= 8190,
 *            u128 n <= 8190, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_log2_uint64.h"   /* floor_log2_uint64(idx_t) */

/* Convert a 1‑based index to the number having only ternary digits 0 and 1,
   interpreting the binary representation of idx as ternary digits. */
static seq_t ternary_no1(idx_t idx)
{
    idx_t len = floor_log2_uint64(idx) + 1;
    seq_t res = 0;
    for (idx_t i = len; i > 0; --i) {
        idx_t bit = (idx >> (i - 1)) & 1;
        res = res * 3 + (seq_t)bit;
    }
    return res;
}

/* Convert a 1‑based index to the number having only ternary digits 0 and 2,
   interpreting the binary representation of idx as ternary digits, with bit 1
   mapped to digit 2. */
static seq_t ternary_no2(idx_t idx)
{
    idx_t len = floor_log2_uint64(idx) + 1;
    seq_t res = 0;
    for (idx_t i = len; i > 0; --i) {
        idx_t bit = (idx >> (i - 1)) & 1;
        res = res * 3 + (seq_t)(bit * 2);
    }
    return res;
}

/* Main sequence function.  It retains a cached list of computed terms and
   expands it as needed on each call.  The cache is dynamically resized
   using malloc/realloc with explicit casts to support arbitrary-precision
   seq_t types. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* not used for the sequence offset */

    static seq_t *cache      = NULL;
    static idx_t  cache_len  = 0;
    static idx_t  cache_cap  = 0;
    static idx_t  seq1_idx   = 1;   /* index into A005823      */
    static idx_t  seq2_idx   = 1;   /* index into A005836      */
    static bool   initialized = false;

    if (!initialized) {
        cache_cap = 256;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        initialized = true;
    }

    /* Generate terms until the requested index is reached. */
    while (cache_len < n) {
        seq_t val1 = ternary_no1(seq1_idx);
        seq_t val2 = ternary_no2(seq2_idx);

        /* Append the smaller next value to the cache. */
        if (val1 <= val2) {
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                cache = tmp;
            }
            cache[cache_len++] = val1;
            ++seq1_idx;
        } else {
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                cache = tmp;
            }
            cache[cache_len++] = val2;
            ++seq2_idx;
        }
    }

    return cache[n - 1];
}
