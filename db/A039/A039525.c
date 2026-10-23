/*
 * A039525  Numbers whose base-12 representation has the same nonzero number
 *          of 3's and 6's.
 * https://oeis.org/A039525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-12 representation has the same nonzero number of 3's and 6's. */
#include <stdint.h>
#include <stdlib.h>

static seq_t* cache = NULL;     /* store the generated sequence */
static size_t cap   = 0;        /* current capacity of cache          */
static size_t len   = 0;        /* number of elements currently stored*/
static seq_t next_value = 0;    /* next integer to test              */
static int initialized = 0;    /* flag for one-time initialization   */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset 1, but handle gracefully */

    /* one‑time setup of the cache */
    if (!initialized) {
        cap = 64;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;   /* allocation failed */
        len = 0;
        next_value = 0;
        initialized = 1;
    }

    /* generate terms until we have at least n of them */
    while (len < n) {
        seq_t curr = next_value;
        seq_t tmp  = curr;
        uint32_t cnt3 = 0, cnt6 = 0;

        /* count digits 3 and 6 in base‑12 */
        while (tmp != 0) {
            uint32_t d = (uint32_t)(tmp % 12);
            if (d == 3) cnt3++;
            else if (d == 6) cnt6++;
            tmp /= 12;
        }

        if (cnt3 > 0 && cnt3 == cnt6) {
            if (len == cap) {                /* grow the cache if needed */
                cap *= 2;
                cache = (seq_t*)realloc(cache, cap * sizeof(seq_t));
            }
            cache[len++] = curr;
        }
        next_value++;     /* advance to the next integer */
    }

    return cache[n-1];   /* 1‑based index */
}
