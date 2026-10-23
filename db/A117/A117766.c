/*
 * A117766  Fibonacci numbers for which the sum of the digits is a Lucas
 *          number.
 * https://oeis.org/A117766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   digit_sum, is_lucas_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117766: Fibonacci numbers whose digit sum is a Lucas
   number (Lucas: 1,3,4,7,11,...).  Computed by iteratively
   generating Fibonacci numbers and keeping those whose digit sum
   belongs to this Lucas sequence. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"
#include "../utils/is_lucas_uint64.h"

/* Append a value to the dynamic cache array. */
static void cache_append(seq_t val, seq_t **cache, size_t *len, size_t *cap)
{
    if (*len >= *cap) {
        size_t newcap = (*cap) ? (*cap) * 2 : 8;
        seq_t *newp = (seq_t *)realloc(*cache, newcap * sizeof(seq_t));
        *cache = newp;
        *cap   = newcap;
    }
    (*cache)[*len] = val;
    (*len)++;
}

/* Public function: returns the nth term of A117766 (0‑based). */
seq_t a(idx_t n)
{
    /* static cache and Fibonacci state */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static seq_t f_prev      = 0;   /* F(k-1)   */
    static seq_t f_curr      = 1;   /* F(k)     */
    static idx_t  fib_idx    = 1;   /* current k */

    /* Build cache until we have at least n+1 terms. */
    while (cache_len <= (size_t)n) {
        /* Digit sum of the current Fibonacci number. */
        seq_t ds   = digit_sum(f_curr);
        uint64_t sum = (uint64_t)ds;              /* sum is small */
        if (is_lucas_uint64(sum)) {
            /* Avoid duplicating 1 (F(1)=1 and F(2)=1). */
            if (cache_len == 0 || f_curr != cache[cache_len - 1]) {
                cache_append(f_curr, &cache, &cache_len, &cache_cap);
            }
        }

        /* Advance to the next Fibonacci number. */
        seq_t next = f_prev + f_curr;
        f_prev    = f_curr;
        f_curr    = next;
        fib_idx++;   /* not used further but kept for clarity */
    }
    return cache[(size_t)n];
}
