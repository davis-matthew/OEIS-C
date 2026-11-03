/*
 * A051803  Numbers with nonzero multiplicative digital root 1.
 * https://oeis.org/A051803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   Numbers with nonzero multiplicative digital root 1 (OEIS
 *            A051803)
 * Range:     u8 n <= 18, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Compute the “nonzero multiplicative digital root” of n.
   0 is treated specially (root 0).  For n>0, repeatedly replace n
   by the product of its non‑zero decimal digits until a single digit
   remains.  The result is that single digit.  */
static inline seq_t mdr(seq_t n)
{
    const seq_t TEN = (seq_t)10;
    if (n == (seq_t)0)
        return (seq_t)0;
    while (n >= TEN) {
        seq_t prod = (seq_t)1;
        seq_t t    = n;
        while (t > (seq_t)0) {
            seq_t d = t % TEN;
            if (d != (seq_t)0)
                prod *= d;
            t /= TEN;
        }
        n = prod;
    }
    return n;
}

/* --- cached generation of the sequence --- */
static idx_t  next_check = 0;   /* next natural number to test */
static idx_t  cache_len  = 0;   /* how many terms have been cached */
static size_t cache_cap  = 0;   /* allocated capacity of cache */
static seq_t *cache      = NULL;

/* Return the nth term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0; /* undefined, but safe fall‑back */

    while (cache_len < n) {
        seq_t val = next_check;
        bool good = (val == (seq_t)0) || (mdr(val) == (seq_t)1);
        if (good) {
            if (cache_cap == 0) {
                cache_cap = 8;
                cache = mem_new(seq_t, cache_cap);
            } else if (cache_len + 1 > cache_cap) {
                cache_cap <<= 1;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = val;
        }
        ++next_check;
    }
    return cache[n - 1];
}
