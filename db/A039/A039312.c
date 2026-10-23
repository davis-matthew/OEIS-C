/*
 * A039312  Numbers whose base-7 representation has the same nonzero number of
 *          0's and 5's.
 * https://oeis.org/A039312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A039312: 
   The nth positive integer whose base‑7 representation
   contains the same positive number of digit 0 and digit 5. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoization ------------------------------------- */
    static seq_t *cache = NULL;
    static size_t   capacity = 0;
    static idx_t    stored   = 0;   /* number of terms already stored  */
    static seq_t    next_candidate = (seq_t)1;  /* first integer to test */

    if (n == 0) return (seq_t)0;   /* safety, though OEIS offset is 1 */

    /* enlarge cache to hold request if necessary ----------------------- */
    if (n > stored) {
        /* at least n terms must be stored */
        size_t needed = n - stored;
        /* grow capacity: start with a non‑zero size, double as needed */
        if (capacity == 0) capacity = 16;
        while (capacity < stored + needed) capacity <<= 1;
        cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));

        /* generate missing terms ------------------------------------- */
        idx_t i = stored;          /* index to fill in cache */
        while (i < n) {
            seq_t candidate = next_candidate;   /* start from where we left off */

            /* find next qualifying number -------------------------------- */
            while (1) {
                seq_t x = candidate;
                unsigned zeros = 0, fives = 0;

                while (x > 0) {
                    unsigned digit = (unsigned)(x % 7);
                    if (digit == 0)           zeros++;
                    else if (digit == 5)      fives++;
                    x /= 7;
                }

                if (zeros == fives && zeros > 0) {
                    cache[i] = candidate;          /* store found term */
                    next_candidate = candidate + 1;/* next starting point   */
                    break;                         /* proceed to next i      */
                }
                candidate++;  /* try next integer */
            }
            ++i;
        }
        stored = n;  /* all requested terms are now stored */
    }

    return cache[n - 1];  /* sequence is 1‑based */
}
