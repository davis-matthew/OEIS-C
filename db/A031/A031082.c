/*
 * A031082  Position of n-th 5 in A031076.
 * https://oeis.org/A031082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   position of the nth occurrence of the digit 5 in the
 *            concatenation of base‑9 representations of the positive
 *            integers. (The sequence A031082.)
 * Range:     u8 n <= 22, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Static state kept between calls.  */
static idx_t       current_num = 1;      /* next integer to process   */
static seq_t       current_pos = 0;      /* position of the next digit*/
static idx_t       count5      = 0;      /* how many 5’s seen so far  */
static seq_t      *cache       = NULL;   /* cache of positions of 5’s*/
static idx_t       cache_len  = 0;       /* number of cached entries  */
static size_t      cache_cap  = 0;       /* cached capacity           */

/* Return the position of the nth occurrence of the digit 5 in the
   concatenated base‑9 representations of the positive integers. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, harmless */

    /* If already cached, just return. */
    if (n <= cache_len)
        return cache[(size_t)(n - 1)];

    /* Extend the cache until we have at least n entries. */
    while (count5 < n) {
        /* Convert current_num to base‑9 digits. */
        idx_t  temp   = current_num;
        int    digits[32];
        int    dlen   = 0;
        while (temp > 0) {
            digits[dlen++] = (int)(temp % 9);
            temp = temp / 9;
        }

        /* Process the digits in most‑significant‑to‑least order. */
        for (int i = dlen - 1; i >= 0; --i) {
            current_pos++;                 /* move to next digit position */
            if (digits[i] == 5) {          /* a 5 is found */
                if (cache_len == (idx_t)cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    /* explicitly cast the result of realloc to seq_t*;
                       this is required for C++ compilation. */
                    seq_t *new_cache = (seq_t *)realloc(cache,
                                               new_cap * sizeof(seq_t));
                    cache = new_cache;
                    cache_cap = new_cap;
                }
                cache[(size_t)cache_len++] = current_pos;
                count5++;                  /* another 5 seen */
            }
        }
        current_num++;                       /* advance to next integer */
    }

    return cache[(size_t)(n - 1)];
}
