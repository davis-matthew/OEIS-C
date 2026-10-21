/*
 * A067493  Powers of 7 with initial digit 1.
 * https://oeis.org/A067493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence:  A067493 – powers of 7 whose first decimal digit is 1.
   a(1)=1, a(2)=7^5, a(3)=7^6, a(4)=7^11, …  (enumerated in order of increasing
   exponent) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* the sequence is 1‑based */

    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0, cache_cap = 0;
    static int initialized = 0;

    /* static state for incremental generation */
    static seq_t current_power = 1;   /* 7^k  (k starts at 0) */
    static idx_t   power_k     = 0;   /* current exponent k */
    static int overflowed = 0;        /* stop further computation once overflow */

    /* one‑time cache allocation */
    if (!initialized) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        initialized = 1;
    }

    /* if we already know all terms that fit, just return */
    if (overflowed) {
        return (n <= cache_size) ? cache[n - 1] : 0;
    }

    /* generate terms until we have at least n or we hit an overflow */
    while (cache_size < (size_t)n) {
        /* --- test the leading decimal digit of current_power --- */
        seq_t tmp = current_power;
        while (tmp >= (seq_t)10) tmp /= (seq_t)10;
        if (tmp == (seq_t)1) {
            /* store it, enlarging the cache if needed */
            if (cache_size == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = current_power;
        }

        /* --- compute next power, checking for overflow for unsigned types --- */
        seq_t next = current_power * (seq_t)7;
        if (next < current_power) {   /* overflow detected */
            overflowed = 1;
            break;
        }

        current_power = next;
        ++power_k;
    }

    /* return the requested term if available */
    return (n <= cache_size) ? cache[n - 1] : 0;
}
