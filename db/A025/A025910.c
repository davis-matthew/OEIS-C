/*
 * A025910  Expansion of 1/((1-x^7)*(1-x^8)*(1-x^11)).
 * https://oeis.org/A025910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrence):
   a(0) = 1
   a(n) = a(n-7)+a(n-8)+a(n-11)-a(n-15)-a(n-18)-a(n-19)+a(n-26)   (n ≥ 1)
   with a(k) = 0 for k < 0
   This function memoises the values in a static array that grows
   as needed.  It works for primitive unsigned types and for the
   arbitrary‑precision seq_t class via the provided allocation helpers.
 */

/* Include the allocation utilities for seq_t arrays (handles
   constructor/destructor for big‑int builds). */
#include "../utils/alloc_seq_array.h"   /* alloc_seq_array(idx_t) returns seq_t* */
#include "../utils/free_seq_array.h"    /* free_seq_array(seq_t*) */

static seq_t *cache = NULL;      /* array of computed values */
static idx_t cache_size = 0;     /* number of elements stored in cache */

/* Compute the nth element of sequence A025910. */
seq_t a(idx_t n)
{
    /*--- Initialisation -------------------------------------------------*/
    if (cache == NULL) {
        cache_size = 1;
        cache = alloc_seq_array(1);      /* allocate array for a(0) only   */
        cache[0] = (seq_t)1;             /* a(0) = 1                       */
    }

    /*--- Expand the cache to include index n ---------------------------*/
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        /* Allocate a new, larger array (default‑constructed elements). */
        seq_t *new_cache = alloc_seq_array(new_size);

        /* Copy existing values to the new array. */
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];

        /* Optionally free the old array (leaking is harmless for the
           required problem size, but we free to keep the memory
           footprint modest). */
        free_seq_array(cache);
        cache = new_cache;

        /*--- Compute new terms using the recurrence -------------------*/
        for (idx_t m = cache_size; m <= n; ++m) {
            seq_t val = (seq_t)0;

            if (m >= 7)  val += cache[m - 7];
            if (m >= 8)  val += cache[m - 8];
            if (m >= 11) val += cache[m - 11];
            if (m >= 15) val -= cache[m - 15];
            if (m >= 18) val -= cache[m - 18];
            if (m >= 19) val -= cache[m - 19];
            if (m >= 26) val += cache[m - 26];

            cache[m] = val;
        }

        cache_size = new_size;
    }

    return cache[n];
}
