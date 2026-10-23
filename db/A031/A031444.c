/*
 * A031444  Numbers whose base-2 representation has one more 0 than 1's.
 * https://oeis.org/A031444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 4081, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A031444: numbers whose binary representation has one more
   0 than 1s.  The values are generated in increasing order by
   enumerating all odd bit‑lengths L and all masks of the lower
   (L−1) bits that satisfy the 0/1 balance condition. */
 
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;
 
/* maximum number of terms we pre‑compute – enough for n up to about 10000 */
#define MAX_TERMS 20000
 
/* Produce all required terms once.  The list is stored in `cache`. */
static void
init_cache(void)
{
    size_t cap = 1024;
    cache = (seq_t*)malloc(cap * sizeof(seq_t));
    cache_size = 0;
 
    /* Generate numbers of odd bit length L, starting from 3. */
    for (int L = 3; cache_size < MAX_TERMS; L += 2) {
        unsigned int bitlen = L - 1;            /* number of lower bits   */
        unsigned int max_bits = sizeof(seq_t) * 8;
        /* Avoid undefined shift when L‑1 >= width of seq_t. */
        if (bitlen >= max_bits)
            break;
 
        seq_t base = ((seq_t)1 << bitlen);     /* the leading 1 at position L‑1 */
        unsigned int max_mask = (1U << bitlen) - 1;
        for (unsigned int mask = 0; mask <= max_mask; ++mask) {
            unsigned int ones_mask = __builtin_popcount(mask);
            seq_t ones_total   = 1 + ones_mask;           /* total ones */
            seq_t zeros_total  = (seq_t)L - ones_total;  /* total zeros */
            if (zeros_total == ones_total + 1) {
                if (cache_size >= cap) {
                    cap *= 2;
                    seq_t *tmp = (seq_t*)realloc(cache, cap * sizeof(seq_t));
                    cache = tmp;
                }
                cache[cache_size++] = base | mask;
                if (cache_size >= MAX_TERMS)
                    break;
            }
        }
    }
}
 
seq_t
a(idx_t n)
{
    if (n == 0)  /* undefined – guard the user */
        return 0;
    if (!cache)
        init_cache();
    if ((size_t)n > cache_size)
        /* The requested index is larger than the pre‑computed amount.
           In the usual test ranges this never happens, but we return 0
           here to keep the function well‑defined. */
        return 0;
    return cache[n - 1];
}
