/*
 * A132351  Partial sums of A132350.
 * https://oeis.org/A132351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = number of positive integers ≤ n that are NOT perfect powers
   (for n ≥ 1).  A perfect power is an integer of the form m^k with m > 1 and k > 1.
   The function uses a static bit array to mark perfect powers up to the
   largest n seen so far; it clears the array for each call and
   recomputes the count.  Complexity is O(√n log n) per call. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    static uint8_t *marked = NULL;
    static idx_t marked_size = 0;

    /* Ensure the array is large enough. */
    if (marked_size <= n) {
        idx_t new_size = n + 1;
        uint8_t *tmp = (uint8_t *)realloc(marked, (size_t)new_size);
        if (!tmp) /* allocation failure – return 0 for safety */
            return (seq_t)0;
        if (marked) {
            /* Initialise the newly added region to zero. */
            memset(tmp + marked_size, 0, (size_t)(new_size - marked_size));
        } else {
            memset(tmp, 0, (size_t)new_size);
        }
        marked = tmp;
        marked_size = new_size;
    }

    /* Clear the array up to n. */
    memset(marked, 0, (size_t)(n + 1));

    idx_t count = 0;          /* number of distinct perfect powers ≤ n (excluding 1) */

    /* Enumerate perfect powers: base ≥ 2, exponent ≥ 2. */
    for (idx_t base = 2; base <= n / base; ++base) {
        idx_t val = base * base; /* start with exponent 2 */
        while (val <= n) {
            if (!marked[val]) {
                marked[val] = 1;
                ++count;
            }
            if (val > n / base)
                break;          /* avoid overflow of val * base */
            val *= base;
        }
    }

    idx_t diff = n - count;   /* count of non‑perfect powers */
    return (seq_t)diff;
}
