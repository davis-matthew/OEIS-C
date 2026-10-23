/*
 * A025617  Numbers of form 4^i*5^j, with i, j >= 0.
 * https://oeis.org/A025617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 35, u32 n <= 125, u64 n <= 470,
 *            u128 n <= 1824, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth smallest integer of the form 4^i * 5^j (i, j ≥ 0).  */
/* The sequence is generated via a two‑pointer merge of the sequences
   4^i and 5^j, keeping all distinct results in ascending order. */

#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

seq_t a(idx_t n)
{
    /* Static cache of computed values and indices into the two sequences. */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t  idx4 = 0;
    static idx_t  idx5 = 0;

    /* Initialize the cache with the first term (1). */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Return directly if we already have enough terms. */
    if (cache_len >= (size_t)n) {
        return cache[(size_t)n - 1];
    }

    /* Determine the width of seq_t for overflow checks. */
    const size_t seq_w = sizeof(seq_t);

    /* Generate terms until we reach the desired index. */
    while (cache_len < (size_t)n) {
        bool overflow4 = true, overflow5 = true;
        seq_t val4 = 0, val5 = 0;

        /* Compute 4 * cache[idx4] if possible. */
        if (idx4 < cache_len) {
            if (seq_w <= 8) {
                uint64_t prod = (uint64_t)cache[idx4] * 4ULL;
                if      (seq_w == 1 && prod <= UINT8_MAX)  { val4 = (seq_t)prod; overflow4 = false; }
                else if (seq_w == 2 && prod <= UINT16_MAX) { val4 = (seq_t)prod; overflow4 = false; }
                else if (seq_w == 4 && prod <= UINT32_MAX) { val4 = (seq_t)prod; overflow4 = false; }
                else if (seq_w == 8 && prod <= UINT64_MAX) { val4 = (seq_t)prod; overflow4 = false; }
                /* For wider types we skip overflow detection. */
            } else {
                val4 = cache[idx4] * (seq_t)4;
                overflow4 = false;
            }
        }

        /* Compute 5 * cache[idx5] if possible. */
        if (idx5 < cache_len) {
            if (seq_w <= 8) {
                uint64_t prod = (uint64_t)cache[idx5] * 5ULL;
                if      (seq_w == 1 && prod <= UINT8_MAX)  { val5 = (seq_t)prod; overflow5 = false; }
                else if (seq_w == 2 && prod <= UINT16_MAX) { val5 = (seq_t)prod; overflow5 = false; }
                else if (seq_w == 4 && prod <= UINT32_MAX) { val5 = (seq_t)prod; overflow5 = false; }
                else if (seq_w == 8 && prod <= UINT64_MAX) { val5 = (seq_t)prod; overflow5 = false; }
            } else {
                val5 = cache[idx5] * (seq_t)5;
                overflow5 = false;
            }
        }

        /* If both candidates overflow, we cannot generate further correct
           terms; stop the loop. */
        if (overflow4 && overflow5) {
            break;
        }

        /* Pick the smallest available candidate. */
        seq_t next;
        if (!overflow4 && !overflow5) {
            if (val4 <= val5) {
                next = val4;
                idx4++;
                if (val4 == val5) idx5++;      /* skip duplicate when equal */
            } else {
                next = val5;
                idx5++;
            }
        } else if (!overflow4) {
            next = val4;
            idx4++;
        } else { /* !overflow5 */
            next = val5;
            idx5++;
        }

        /* Append the new value if it is distinct from the previous one. */
        if (cache_len == 0 || next != cache[cache_len - 1]) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next;
        }
    }

    /* Return the requested term if available, otherwise the last computed
       value (used when seq_t is too small to hold further terms). */
    if (cache_len >= (size_t)n) {
        return cache[(size_t)n - 1];
    } else {
        return cache[cache_len - 1];
    }
}
