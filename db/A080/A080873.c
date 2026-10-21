/*
 * A080873  a(n)*a(n+3) - a(n+1)*a(n+2) = 5, given a(0)=a(1)=1, a(2)=2.
 * https://oeis.org/A080873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 10*a(n-2) - a(n-4)  (for n ≥ 4)
   Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=7
   The recurrence is equivalent to a(n+4)=10*a(n+2)-a(n). */
#include <stdlib.h>

static seq_t *cache = NULL;      /* dynamically growable cache of terms   */
static idx_t allocated = 0;      /* number of terms currently cached     */

/* Ensure that cache contains terms up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n < allocated) return;                 /* already computed */

    /* Decide new size: at least n+1, grow at least twice. */
    idx_t new_size = allocated ? allocated << 1 : 8;
    if (new_size <= n) new_size = n + 1;

    /* Reallocate the array. */
    cache = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));

    /* Initialise the first four terms if this is the first allocation. */
    if (allocated == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)7;
    }

    /* Compute new terms using the linear recurrence. */
    idx_t start = (allocated > 4) ? allocated : 4;
    for (idx_t i = start; i < new_size; ++i) {
        cache[i] = ((seq_t)10 * cache[i-2]) - cache[i-4];
    }

    allocated = new_size;
}

/* External function required by the build. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
