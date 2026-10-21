/*
 * A081071  a(n) = Lucas(4*n+2)-2 = Lucas(2*n+1)^2.
 * https://oeis.org/A081071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(4*n+2) – 2 (Lucas numbers are defined by * L₀ = 2,
 *            L₁ = 1, Lₙ = Lₙ₋₁ + Lₙ₋₂). The array lucas_cache stores * Lᵢ for
 *            i ≥ 0. Ensuring that entries up to the required index * exist is
 *            fast enough for n ≈ 10 000; the recurrence is O(k) where * k =
 *            4*n+2, which is only about 40 000 iterat...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Global cache for Lucas numbers. */
static idx_t lucas_max_computed = 0;   /* highest index that has been computed */
static seq_t *lucas_cache = NULL;

/* Expand the cache so that lucas_cache[max_index] is valid.  */
static void ensure_lucas(idx_t max_index)
{
    if (max_index <= lucas_max_computed)
        return;                           /* already available */

    size_t new_size = (size_t)(max_index + 1);   /* indices 0 .. max_index */
    seq_t *new_cache = (seq_t *)realloc(lucas_cache, new_size * sizeof(seq_t));
    if (!new_cache) abort();                /* allocation failed */

    lucas_cache = new_cache;

    /* First allocation: initialise base Lucas values. */
    if (lucas_max_computed == 0) {
        new_cache[0] = (seq_t)2;
        new_cache[1] = (seq_t)1;
        lucas_max_computed = 1;
    }

    /* Compute new terms only from the next index onward.  */
    for (idx_t i = lucas_max_computed + 1; i <= max_index; ++i)
        new_cache[i] = new_cache[i - 1] + new_cache[i - 2];

    lucas_max_computed = max_index;
}

/* External linkage function demanded by the harness. */
seq_t a(idx_t n)
{
    idx_t target = 4 * n + 2;          /* index of the desired Lucas number */
    ensure_lucas(target);
    return lucas_cache[target] - (seq_t)2;
}
