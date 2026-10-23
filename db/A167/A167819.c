/*
 * A167819  Numbers with a distinct frequency for each ternary digit.
 * https://oeis.org/A167819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A167819: numbers whose base‑3 representation uses each
 * ternary digit 0,1,2 a distinct number of times.  The function
 * lazily builds the sequence up to the needed index n and then
 * returns a(n) (1‑based). */

/* Explanation:
 *   We maintain a dynamically grown array of sequence terms.  Whenever a
 *   new term is required we scan successive integers, test whether the
 *   counts of digits 0,1,2 are all distinct, and store those that
 *   satisfy the condition.  We stop once we have n terms.
 *   The test works by repeatedly dividing the candidate number by 3
 *   and accumulating the digit counts.  Division, modulo, increments
 *   and comparisons are guaranteed to be supported by both the
 *   fixed‑size integer types (uint8_t … uint64_t, unsigned __int128)
 *   and the arbitrary‑precision integer class used when the code
 *   is compiled as C++17. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Lazy static state for memoisation -------------------------- */
    static seq_t *cache = NULL;      /* array of computed terms   */
    static size_t cache_cap = 0;     /* current array capacity   */
    static size_t cache_size = 0;    /* number of valid entries   */
    static seq_t next_candidate = 1; /* smallest integer to test  */

    /* Helper to grow the array when needed ---------------------- */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Fill the cache until we have at least n terms each time. */
    while (cache_size < (size_t)n) {
        seq_t v = next_candidate;
        next_candidate++;

        /* Count digits of v in base 3.  v == 0 never enters the
         * sequence (its counts are all equal).  We simply skip it
         * because the loop above never starts with v == 0. */
        unsigned int cnt[3] = {0, 0, 0};
        seq_t tmp = v;
        while (tmp != 0) {
            unsigned digit = (unsigned)(tmp % 3);
            cnt[digit]++;
            tmp /= 3;
        }

        /* Record if the counts for 0,1,2 are pairwise distinct. */
        if (cnt[0] != cnt[1] && cnt[0] != cnt[2] && cnt[1] != cnt[2]) {
            /* Grow array if required. */
            if (cache_size == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = v;
        }
    }

    /* Return the n‑th term (1‑based indexing). */
    return cache[(size_t)n - 1];
}
