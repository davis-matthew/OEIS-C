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
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static void ordinal_word(idx_t n, char *buf)
{
    /* n >= 2.  The buffer must be large enough (64 bytes is ample).  */
    static const char *unit_ord[10] =
        {"", "first", "second", "third", "fourth",
         "fifth", "sixth", "seventh", "eighth", "ninth"};
    static const char *ten_special[10] =
        {"tenth", "eleventh", "twelfth", "thirteenth", "fourteenth",
         "fifteenth", "sixteenth", "seventeenth", "eighteenth", "nineteenth"};
    static const char *tens_word[10] =
        {"", "", "twenty", "thirty", "forty",
         "fifty", "sixty", "seventy", "eighty", "ninety"};
    static const char *ten_ord[10] =
        {"", "", "twentieth", "thirtieth", "fortieth",
         "fiftieth", "sixtieth", "seventieth", "eightieth", "ninetieth"};

    if (n < 10) {
        strcpy(buf, unit_ord[(int)n]);
    } else if (n < 20) {
        strcpy(buf, ten_special[(int)n - 10]);
    } else {
        int tens  = (int)((n / 10) * 10);
        int unit  = (int)( n % 10 );
        if (unit == 0) {
            strcpy(buf, ten_ord[tens / 10]);
        } else {
            strcpy(buf, tens_word[tens / 10]);
            strcat(buf, unit_ord[unit]);
        }
    }
}

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
        cache = mem_new(seq_t, cache_cap);
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
                    cache = mem_resize(seq_t, cache, new_cap);
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
                    cache = mem_resize(seq_t, cache, new_cap);
                    cache_cap = new_cap;
                }
                cache[++cache_size] = (seq_t)pos;
            }
        }
        ++cur_ord;      /* next word of the sentence */
    }

    return cache[n];   /* a(1) is cache[1]  (1‑based indexing) */
}
