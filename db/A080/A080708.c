/*
 * A080708  a(1)=5; for n>1, a(n) = smallest number > a(n-1) such that the
 *          condition "n is in the sequence if and only if a(n) is a multiple
 *          of 4" is satisfied.
 * https://oeis.org/A080708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A080708: 
 * a(1) = 5;
 * for n > 1, a(n) is the smallest integer > a(n-1) such that “n is in the sequence if and only if a(n) is a multiple of 4”.
 * The implementation builds the sequence incrementally, using the fact that an index n belongs to the sequence exactly when it has already appeared as a value earlier.
 * The algorithm runs in O(n) time and uses only integer arithmetic. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Static cache for generated terms */
static seq_t *seq_cache = NULL;     /* dynamic array of terms a(1), a(2), … */
static size_t seq_used = 0;         /* number of terms currently stored */
static size_t seq_cap  = 0;         /* allocated capacity of seq_cache */

/* Static array marking which integers have already appeared as values */
static uint8_t *seen = NULL;        /* true if the integer has already been a term */
static size_t seen_cap = 0;         /* allocated size of seen array */

/* Expand the seq_cache to hold at least 'cap' elements. */
static void expand_seq(size_t cap)
{
    if (cap <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    if (new_cap < cap)
        new_cap = cap;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Expand the seen array to hold at least indices 0 … idx. */
static void expand_seen(size_t idx)
{
    if (idx < seen_cap)
        return;
    size_t new_cap = seen_cap ? seen_cap * 2 : 64;
    if (new_cap <= idx)
        new_cap = idx + 1;
    uint8_t *new_arr = (uint8_t *)realloc(seen, new_cap * sizeof(uint8_t));
    /* initialize newly allocated region to 0 */
    for (size_t i = seen_cap; i < new_cap; ++i)
        new_arr[i] = 0;
    seen = new_arr;
    seen_cap = new_cap;
}

/* Main function to return a(n).  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n=0; safeguard */

    /* Generate terms up to n if necessary */
    while (seq_used < (size_t)n) {
        seq_t val;
        if (seq_used == 0) {
            /* Base term */
            val = (seq_t)5;
        } else {
            seq_t prev = seq_cache[seq_used - 1];
            idx_t idx = (idx_t)(seq_used + 1);   /* current index n */

            /* Determine if idx has appeared before as a value */
            bool in_sequence = false;
            if ((size_t)idx < seen_cap)
                in_sequence = seen[(size_t)idx] != 0;

            if (in_sequence) {
                /* We need a multiple of 4 greater than prev */
                if (prev % (seq_t)4 == (seq_t)0)
                    val = prev + (seq_t)4;
                else
                    val = ((prev / (seq_t)4) + (seq_t)1) * (seq_t)4;
            } else {
                /* We need a non‑multiple of 4 greater than prev */
                val = prev + (seq_t)1;
                if (val % (seq_t)4 == (seq_t)0)
                    val = val + (seq_t)1;
            }
        }

        /* Ensure the caches are large enough */
        expand_seq(seq_used + 1);

        /* Mark that 'val' has now appeared as a value */
        size_t val_index = (size_t)val;
        if (val_index >= seen_cap)
            expand_seen(val_index);
        seen[val_index] = 1;

        /* Store the term */
        seq_cache[seq_used] = val;
        seq_used++;
    }

    /* Return the requested term (1‑based index) */
    return seq_cache[(size_t)n - 1];
}
