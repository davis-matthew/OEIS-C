/*
 * A067818  Start with 1. To get a(n+1), describe a(n) in terms of the number
 *          of 0's, 1's, 2's,...,9's it has. Do not ignore leading 0's in the
 *          computation, but ignore them when listing the terms.
 * https://oeis.org/A067818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 12,
 *            big any n
 * Helpers:   dup_str, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(1) = 1.
    For n > 1: write a(n-1) as a decimal string, counting each digit 0–9
    (including any leading zeros that were present in that string).
    For each digit d from 0 to 9 concatenate the decimal representation
    of that count followed by the digit d.
    The resulting string is the next term a(n).  Leading zeros are kept
    when counting for the following term, but they are omitted when
    the term is returned.  */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../utils/seq_from_str.h"
#include "../utils/dup_str.h"

static seq_t *cache      = NULL;   /* cache[1] .. cache[lastComputed]  */
static size_t cache_cap  = 0;       /* allocated size of cache          */
static idx_t  lastComputed = 0;     /* largest index computed so far    */
static char  *last_str    = NULL;   /* decimal string of last term      */

static void ensure_cache(idx_t n)
{
    if (cache == NULL) {                    /* first call: initialise a(1)=1 */
        cache_cap = 2;                      /* capacity for indices 1 and 2 at least */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[1] = seq_from_str("1");
        last_str   = dup_str("1");
        lastComputed = 1;
    }

    while (lastComputed < n) {
        unsigned long long counts[10] = {0};

        /* count digits in last_str (including any leading zeros) */
        for (size_t i = 0; last_str[i] != '\0'; ++i) {
            char c = last_str[i];
            if (c >= '0' && c <= '9')
                counts[c - '0']++;
        }

        /* determine length of next string */
        size_t total_len = 0;
        char  tmp_buf[32];
        for (int d = 0; d <= 9; ++d) {
            int len = snprintf(tmp_buf, sizeof(tmp_buf), "%llu",
                               (unsigned long long)counts[d]);
            total_len += (size_t)len + 1;   /* +1 for the digit character */
        }

        /* build the next string */
        char *next_str = (char *)malloc(total_len + 1);  /* +1 for NUL */
        char *p = next_str;
        for (int d = 0; d <= 9; ++d) {
            int len = snprintf(tmp_buf, sizeof(tmp_buf), "%llu",
                               (unsigned long long)counts[d]);
            memcpy(p, tmp_buf, (size_t)len);
            p += len;
            *p++ = '0' + d;
        }
        *p = '\0';

        /* convert to seq_t value and store */
        seq_t next_val = seq_from_str(next_str);
        if ((size_t)(lastComputed + 1) >= cache_cap) {   /* grow cache */
            size_t new_cap = cache_cap * 2;
            if (new_cap == 0) new_cap = 2;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }
        cache[lastComputed + 1] = next_val;

        /* advance to next term */
        free(last_str);
        last_str = next_str;
        ++lastComputed;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;     /* undefined for n < 1, keep defensive */
    ensure_cache(n);
    return cache[n];
}
