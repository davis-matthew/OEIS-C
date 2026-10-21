/*
 * A222332  Number of binary arrays indicating the locations of trailing edge
 *          maxima of a random length-n 0..6 array extended with zeros and
 *          convolved with 1,1.
 * https://oeis.org/A222332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-5) + a(n-7) + a(n-9) + a(n-11) +
 *            a(n-13) * (OEIS recurrence with a(1)=2, a(2)=3, …, a(13)=521)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Base values for 1 ≤ n ≤ 13 – these come directly from the OEIS entry. */
static const seq_t base_vals[14] = {
    (seq_t)0,   /* unused 0‑th entry */
    (seq_t)2,   (seq_t)3,   (seq_t)4,   (seq_t)7,
    (seq_t)11,  (seq_t)18,  (seq_t)29,  (seq_t)47,
    (seq_t)76,  (seq_t)123, (seq_t)199, (seq_t)322,
    (seq_t)521
};

/* Cached values.  cache[0] is never accessed (it just keeps the pointer
 * layout simple).  cache[i] holds a(i) for 1 ≤ i ≤ cache_size. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0; /* allocated slots */
static size_t cache_size = 0; /* highest index already computed */

/* Ensure the cache can hold at least `need` entries (including 0‑th entry). */
static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < need) new_cap <<= 1;

    /* Use an explicit cast so the code compiles as C++ as well. */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute the n‑th term of A222332.  The function works for any
 * sequence type (fixed width unsigned or arbitrary‑precision integer)
 * and any order of calls. */
seq_t a(idx_t n)
{
    if (n <= 13) return base_vals[n];

    /* Lazily initialise the cache on first call. */
    if (!cache) {
        grow_cache(64);
        cache[0] = (seq_t)0;          /* dummy entry */
        for (size_t i = 1; i <= 13; ++i) cache[i] = base_vals[i];
        cache_size = 13;
    }

    size_t idx = (size_t)n;
    if (idx > cache_size) {
        grow_cache(idx + 1);           /* ensure space for a(idx) */

        for (size_t i = cache_size + 1; i <= idx; ++i) {
            seq_t val = cache[i - 1];
            if (i >= 3) val += cache[i - 3];
            if (i >= 5) val += cache[i - 5];
            if (i >= 7) val += cache[i - 7];
            if (i >= 9) val += cache[i - 9];
            if (i >= 11) val += cache[i - 11];
            if (i >= 13) val += cache[i - 13];
            cache[i] = val;
        }
        cache_size = idx;
    }
    return cache[idx];
}
