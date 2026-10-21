/*
 * A072886  The s-aints, numbers generated like the Aronson series from a
 *          generating sentence, "S ain't the second, third, fourth, fifth . .
 *          . letter of this sentence.".
 * https://oeis.org/A072886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   a(n) is the index of the nth letter * that is not the letter ‘s’
 *            (case‑insensitive) in the infinitely * long sentence * * "S
 *            ain't the second, third, fourth, fifth, sixth, seventh, eighth,
 *            * ninth, tenth, eleventh, twelfth, thirteenth, fourteenth,
 *            fifteenth, * sixteenth, seventeenth, eighteenth,...
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   ordinal_word
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ordinal_word.h"

seq_t a(idx_t n)
{
    /* Memoisation storage */
    static seq_t *cache = NULL;     /* positions of non‑'s letters   */
    static size_t cache_cap = 0;    /* current capacity of cache      */
    static idx_t cache_size = 0;    /* number of cached values (1‑based) */

    /* Generation state */
    static idx_t pos = 0;            /* current letter position in sentence (1‑based)  */
    static int  prefix_idx = 0;      /* 0 … 7  (prefix:"SAINTTHE")                    */
    static idx_t cur_ord = 2;        /* next ordinal number to write                   */

    /* Initialise cache if needed on the first call */
    if (!cache) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Keep generating until we have at least n values cached.  */
    while (cache_size < n) {
        /* -------  prefix letters:  "S"  +  "aint"  +  "the"  -------- */
        static const char prefix[8] = {'S', 'a', 'i', 'n', 't', 't', 'h', 'e'};
        if (prefix_idx < 8) {
            ++pos;
            char c = prefix[prefix_idx++];
            if (c != 's' && c != 'S') {
                /* enlarge cache if necessary */
                if (cache_size + 1 > cache_cap) {
                    size_t new_cap = cache_cap * 2;
                    if (new_cap < cache_size + 1) new_cap = cache_size + 1;
                    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    cache_cap = new_cap;
                }
                cache[++cache_size] = (seq_t)pos;
            }
            continue;
        }

        /* -------  generate the current ordinal word  -------- */
        char buf[64];
        ordinal_word(cur_ord, buf);
        for (char *p = buf; *p; ++p) {
            ++pos;
            if (*p != 's' && *p != 'S') {
                if (cache_size + 1 > cache_cap) {
                    size_t new_cap = cache_cap * 2;
                    if (new_cap < cache_size + 1) new_cap = cache_size + 1;
                    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    cache_cap = new_cap;
                }
                cache[++cache_size] = (seq_t)pos;
            }
        }
        ++cur_ord;      /* next word of the sentence */
    }

    return cache[n];   /* a(1) is cache[1]  (1‑based indexing) */
}
