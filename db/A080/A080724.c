/*
 * A080724  a(0) = 2; for n>0, a(n) is taken to be the smallest positive
 *          integer greater than a(n-1) which is consistent with the condition
 *          "n is a member of the sequence if and only if a(n) == 1 mod 3".
 * https://oeis.org/A080724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 2,   a(a(n)) = 3*n + 4   for n ≥ 0 */
#include <stdlib.h>

static seq_t *cache = NULL;      /* array for a(n) */
static idx_t cache_size = 0;    /* current allocated size (number of elements) */
static idx_t computed_up_to = 0; /* the largest n that has been fully computed */

/* Ensure cache has at least (size) elements, initialise new slots to 0 */
static void ensure_cache_size(idx_t size)
{
    if (size <= cache_size) return;

    idx_t new_size = size;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_ptr) {
        /* allocation failed – abort the program */
        abort();
    }
    /* initialise newly allocated elements to 0 */
    for (idx_t i = cache_size; i < new_size; ++i) {
        new_ptr[i] = (seq_t)0;
    }
    cache = new_ptr;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    /* One‑time initialisation for a(0) */
    if (!cache) {
        cache_size = 1;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)2;            /* a(0) = 2 */

        /* a(a(0)) = 4, i.e. a[2] = 4 */
        idx_t idx = (idx_t)cache[0];
        ensure_cache_size(idx + 1);
        cache[idx] = (seq_t)(3 * (seq_t)0 + 4);
        computed_up_to = 0;
    }

    /* If the requested n is beyond current allocation, enlarge cache */
    if (n >= cache_size)
        ensure_cache_size(n + 1);

    /* Build the sequence up to n if necessary */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        if (cache[i] == (seq_t)0) {
            /* minimal value larger than a(i‑1) */
            cache[i] = cache[i - 1] + (seq_t)1;
        }

        /* Compute a(a(i)) = 3*i + 4 and store it at index a(i) */
        seq_t target = (seq_t)i * 3 + (seq_t)4;
        idx_t target_index = (idx_t)cache[i];

        ensure_cache_size(target_index + 1);
        if (cache[target_index] == (seq_t)0) {
            cache[target_index] = target;
        } else {
            /* The value should match the required one by the recurrence.
               If it doesn't, the sequence definition is inconsistent, but
               this situation cannot occur for valid inputs.            */
        }
    }

    computed_up_to = n;
    return cache[n];
}
