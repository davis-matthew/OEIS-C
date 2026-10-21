/*
 * A213699  Half the number of 4X4 0..n symmetric arrays with no 2X2 subblock
 *          summing to 2n.
 * https://oeis.org/A213699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            3*a(n-1)+5*a(n-2)-23*a(n-3)-4*a(n-4)+76*a(n-5)-28*a(n-6)-140*a(n-7)
 *            +98*a(n-8)+154*a(n-9)-154*a(n-10)-98*a(n-11)+140*a(n-12)+28*a(n-13)
 *            -76*a(n-14)+4*a(n-15)+23*a(n-16)-5*a(n-17)-3*a(n-18)+a(n-19)
 *            Initial terms (n=1..19): 70, 7827, 208072, 2322599, 16537086,
 *            84059234, 342943680, 1169405354...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 9, u64 n <= 89, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_capacity = 0;

static void init_cache(void)
{
    /* Ensure at least 19 slots for the seed values. */
    if (cache_capacity < 19) {
        cache_capacity = 19;
    }
    if (!cache) {
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache) return;  /* allocation failure – graceful exit */
        cache[0]  = 70;
        cache[1]  = 7827;
        cache[2]  = 208072;
        cache[3]  = 2322599;
        cache[4]  = 16537086;
        cache[5]  = 84059234;
        cache[6]  = 342943680;
        cache[7]  = 1169405354;
        cache[8]  = 3500184570;
        cache[9]  = 9372540629;
        cache[10] = 23022951048;
        cache[11] = 52423261153;
        cache[12] = 112270651514;
        cache[13] = 227577256564;
        cache[14] = 440658095360;
        cache[15] = 818409534692;
        cache[16] = 1466948929086;
        cache[17] = 2544850623111;
        cache[18] = 4291394833480;
        cache_size = 19;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    if (!cache) init_cache();
    if (!cache) return 0;  /* allocation failure */

    /* Convert to 0‑based index for the internal array. */
    idx_t idx_needed = n - 1;

    while (cache_size <= (size_t)idx_needed) {
        /* Grow the buffer if necessary. */
        if (cache_size == cache_capacity) {
            size_t new_capacity = cache_capacity * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
            if (!tmp) return 0;  /* allocation failure */
            cache = tmp;
            cache_capacity = new_capacity;
        }

        /* i is the index of the last stored term a(i+1). */
        size_t i = cache_size - 1;

        /* Compute a(i+2) using the recurrence. */
        seq_t pos = 3 * cache[i]
                 + 5 * cache[i - 1]
                 + 76 * cache[i - 4]
                 + 98 * cache[i - 7]
                 + 154 * cache[i - 8]
                 + 140 * cache[i - 11]
                 + 28 * cache[i - 12]
                 + 4 * cache[i - 14]
                 + 23 * cache[i - 15]
                 + cache[i - 18];

        seq_t neg = 23 * cache[i - 2]
                 + 4 * cache[i - 3]
                 + 28 * cache[i - 5]
                 + 140 * cache[i - 6]
                 + 154 * cache[i - 9]
                 + 98 * cache[i - 10]
                 + 76 * cache[i - 13]
                 + 5 * cache[i - 16]
                 + 3 * cache[i - 17];

        cache[cache_size++] = pos - neg;
    }

    return cache[(size_t)(n - 1)];
}
