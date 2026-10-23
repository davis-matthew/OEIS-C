/*
 * A044572  Numbers n such that string 0,4 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has04oct
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044572: numbers n such that the octal representation of n
 * contains the substring "04" but n+1 does not.  This implementation
 * searches over successive integers, storing found terms in a
 * dynamically resizable array.  The function is fully memoised, so
 * calls in any order are efficient. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has04oct.h"

/* Ensure that *arr points to memory for at least <need> elements.
 * The allocated capacity is increased to the next power of two ≥ need.
 * The function works for any seq_t type, including arbitrary‑precision
 * classes, as the cast is explicit. */
static void
ensure_capacity(size_t need, size_t *cap, seq_t **arr)
{
    if (*cap < need) {
        size_t newcap = (*cap == 0) ? 1 : (*cap << 1);
        while (newcap < need)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        if (tmp) {              /* realloc succeeded */
            *arr = tmp;
            *cap = newcap;
        }
    }
}

/* Return the n-th term of the sequence (offset 1).  The function
 * computes terms lazily and caches them for future use. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n < 1, return 0 for safety */

    static seq_t *terms      = NULL;   /* stored sequence values */
    static size_t terms_len  = 0;      /* number of terms stored   */
    static size_t terms_cap  = 0;      /* allocated capacity       */
    static uint64_t cur      = 1;      /* next integer to test     */

    while (terms_len < (size_t)n) {
        if (has04oct(cur) && !has04oct(cur + 1)) {
            ensure_capacity(terms_len + 1, &terms_cap, &terms);
            terms[terms_len++] = (seq_t)cur;
        }
        ++cur;
    }
    return terms[(size_t)n - 1];
}
