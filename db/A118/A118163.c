/*
 * A118163  Start with 1 and repeatedly reverse the digits and add 65 to get
 *          the next term.
 * https://oeis.org/A118163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 26, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   ensure_capacity, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the recurrence
   a(1) = 1
   a(n) = reverse_digits(a(n-1)) + 65   for n > 1  */
#include <stddef.h>
#include "../utils/reverse_digits.h"
#include "../utils/ensure_capacity.h"

static seq_t *cache = NULL;   /* array of already computed terms */
static size_t cap   = 0;      /* current allocated capacity of cache */
static size_t len   = 0;      /* number of terms actually stored */

/* Return the n-th term of the sequence (offset 1).  */
seq_t a(idx_t n)
{
    /* Fast return if we already know the term. */
    if (n <= len) {
        return cache[(size_t)(n - 1)];
    }

    /* Guarantee space for at least n terms. */
    ensure_capacity((size_t)n, &cap, (void **)&cache, sizeof(seq_t));

    /* Initialize the very first term if not done yet. */
    if (len == 0) {
        cache[0] = (seq_t)1;
        len = 1;
    }

    /* Build terms from current length up to the requested one. */
    for (idx_t i = len; i < n; ++i) {
        seq_t prev = cache[(size_t)(i - 1)];
        seq_t rev  = reverse_digits(prev);
        cache[(size_t)i] = rev + (seq_t)65;
    }
    len = (size_t)n;

    return cache[(size_t)(n - 1)];
}
