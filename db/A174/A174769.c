/*
 * A174769  x-values in the solution to x^2 - 29*y^2 = 1.
 * https://oeis.org/A174769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19602*a(n-1)-a(n-2); a(1)=1, a(2)=9801
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t computed = 0;

/* Ensure that cache contains terms up to index n (1‑based). */
static void ensure_up_to(idx_t n)
{
    if (n <= computed) return;

    size_t new_size = (size_t)n;
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

    /* Initialise the first one or two terms if they are not yet set. */
    if (computed == 0) {
        cache[0] = 1;
        computed = 1;
        if (n >= 2) {
            cache[1] = 9801;
            computed = 2;
        }
    } else if (computed == 1) {
        if (n >= 2) {
            cache[1] = 9801;
            computed = 2;
        }
    }

    /* Compute the remaining terms using the recurrence. */
    for (idx_t i = computed; i < n; ++i) {
        seq_t a_nm1 = cache[i - 1];
        seq_t a_nm2 = cache[i - 2];
        cache[i] = 19602 * a_nm1 - a_nm2;
    }
    computed = n;
}

/* Public sequence function as required. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1, but harmless */
    ensure_up_to(n);
    return cache[n - 1];
}
