/*
 * A240829  a(1)=-1, a(2)=0, a(3)=1; thereafter a(n) =
 *          Sum(a(n-i-s-a(n-i-1)),i=0..k-1) where s=0, k=3.
 * https://oeis.org/A240829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, hear
 * Formula:   not stated
 * Range:     i8 n <= 187, i16 n <= 9474, i32 n <= 9474, i64 n <= 9474,
 *            i128 n <= 9474, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A240829
   a(1) = -1, a(2) = 0, a(3) = 1
   for n > 3:
   a(n) = Σ_{i=0}^{2} a(n – i – a(n – i – 1))   (s = 0, k = 3)
   The function uses memoisation to allow arbitrary order of calls. */

#include <stdlib.h>

/* The types seq_t and idx_t are supplied by the build system.  */

static seq_t *cache = NULL;   /* cache[0] unused, cache[n] = a(n) for n ≥ 1 */
static size_t cap   = 0;      /* allocated array size (cap ≥ 0) */
static idx_t  computed = 0;   /* highest index that has been computed */

/* Ensure that cache has room for indices up to min_size (inclusive). */
static void ensure_cache(idx_t min_size)
{
    if ((size_t)min_size + 1 <= cap)
        return;

    size_t new_cap = cap ? cap * 2 : 16;
    while (new_cap <= (size_t)min_size)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));
    if (!new_cache)
        return;                    /* allocation failure – not expected */

    if (cache) {
        for (size_t i = 0; i <= (size_t)computed; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }

    cache = new_cache;
    cap   = new_cap;
}

/* Compute all terms up to and including n.  After the call, cache[n] is defined. */
static void compute_up_to(idx_t n)
{
    if (n <= computed)
        return;

    ensure_cache(n);

    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t val;

        if (i == 1)
            val = -1;
        else if (i == 2)
            val = 0;
        else if (i == 3)
            val = 1;
        else {
            seq_t sum = 0;
            for (idx_t j = 0; j < 3; ++j) {
                idx_t prev_idx = i - j - 1;
                seq_t prev_val = 0;
                if (prev_idx >= 1)
                    prev_val = cache[(size_t)prev_idx];

                /* target index = n – i – a(n – i – 1) */
                seq_t target_int = (seq_t)i - (seq_t)j - prev_val;
                if (target_int > 0) {
                    idx_t target = (idx_t)target_int;
                    sum += cache[(size_t)target];
                }
            }
            val = sum;
        }

        cache[(size_t)i] = val;
    }

    computed = n;
}

/* Public interface required by the build system. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                        /* undefined for below offset */

    compute_up_to(n);
    return cache[(size_t)n];
}
