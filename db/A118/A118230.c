/*
 * A118230  Column 3 of triangle A118229, starting with row 1 to include
 *          leading zeros.
 * https://oeis.org/A118230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 0 and for n ≥ 2, a(n‑1) = δ_{n,4} – ∑_{k=1}^{n‑2,\
 *            gcd(k,n)=1} a(k) where δ_{n,4} = 1 if n = 4 else 0
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* idx_t and seq_t are defined by the build environment. */
static seq_t *cache = NULL;          /* dynamic array of sequence values */
static idx_t cache_size = 0;         /* allocated size of cache */
static idx_t computed_max = 0;       /* largest index for which a[i] is known */

/* Euclidean algorithm for unsigned 64‑bit integers. */
static inline idx_t my_gcd(idx_t a, idx_t b)
{
    while (b != (idx_t)0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Ensure cache can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;
    idx_t new_size = cache_size ? cache_size * 2 : 1;
    while (new_size <= n) new_size <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) { /* allocation failed, abort */
        abort();
    }
    cache = new_cache;
    cache_size = new_size;
    /* previous values are preserved; new slots are uninitialized */
}

/* OEIS sequence A118230 */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;   /* behaviour below the offset is undefined */

    /* allocate and extend cache as needed */
    ensure_cache(n);

    /* compute missing values up to index n if necessary */
    if (computed_max < n) {
        /* a[1] is the first value; we compute values using the
           recurrence formula derived above.
           For each cur from computed_max+1 to n+1,
           we compute a[cur-1] (the new value). */
        for (idx_t cur = computed_max + 1; cur <= n + 1; ++cur) {
            seq_t sum = (seq_t)0;
            /* sum over k = 1 .. cur-2 where gcd(k,cur)=1 */
            for (idx_t k = (idx_t)1; k < cur - 1; ++k) {
                if (my_gcd(k, cur) == (idx_t)1) {
                    sum += cache[k];
                }
            }
            seq_t delta = (cur == (idx_t)4) ? (seq_t)1 : (seq_t)0;
            seq_t value = delta - sum;
            cache[cur - 1] = value;
        }
        computed_max = n;
    }

    return cache[n];
}
