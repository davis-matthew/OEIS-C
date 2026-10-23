/*
 * A043580  Numbers whose base-2 representation has exactly 13 runs.
 * https://oeis.org/A043580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-2 representation has exactly 13 runs. Computed by
   brute‑force search. */
#include <stdlib.h>

static seq_t *cache = NULL;     /* cached a(1)..a(k) */
static idx_t cache_size = 0;    /* how many have been cached */
static seq_t last_value = 0;    /* last number tested */

seq_t a(idx_t n)
{
    /*  The OEIS offset is 1 – a(1) is the first term.  */
    if (n == 0) return 0;      /* undefined behaviour below the offset */

    /* Extend the cache if necessary.  */
    if (cache_size < n) {
        /* Allocate space for the new values.  */
        seq_t *tmp = (seq_t*)realloc(cache, n * sizeof(seq_t));
        if (!tmp) {            /* allocation failed – return 0 */
            return 0;
        }
        cache = tmp;

        seq_t val = last_value;      /* start searching from previous value */
        while (cache_size < n) {
            val = val + 1;           /* next integer to test      */

            /* Count runs in the binary representation of val.  */
            seq_t x = val;
            seq_t prev_bit = x & 1;
            int runs = 1;
            x >>= 1;
            while (x) {
                seq_t bit = x & 1;
                if (bit != prev_bit) {
                    runs++;
                    prev_bit = bit;
                }
                x >>= 1;
            }

            if (runs == 13) {
                cache[cache_size++] = val;
            }
        }
        last_value = val;
    }

    return cache[n-1];
}
