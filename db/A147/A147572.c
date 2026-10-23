/*
 * A147572  Numbers with exactly 5 distinct prime divisors {2,3,5,7,11}.
 * https://oeis.org/A147572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2310 * A051038(n) (A147572)
 * Range:     u8 none, u16 n <= 23, u32 n <= 2896, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* primes that may appear in the smooth numbers */
static const seq_t primes[5] = { (seq_t)2, (seq_t)3, (seq_t)5,
                                 (seq_t)7, (seq_t)11 };

/* static storage for the generated A051038 values (1‑based) */
static seq_t *smooth = NULL;      /* array of A051038 values */
static size_t smooth_cap = 0;     /* current capacity of the array */
static size_t smooth_len = 0;     /* number of generated elements */

/* pointers into smooth for each prime multiplier */
static size_t ptrs[5] = {0,0,0,0,0};

/* enlarge the smooth array if necessary */
static void grow_smooth(void)
{
    if (smooth_len >= smooth_cap) {
        size_t new_cap = (smooth_cap == 0) ? 16 : smooth_cap * 2;
        seq_t *tmp = (seq_t *)realloc(smooth, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failed */
        smooth = tmp;
        smooth_cap = new_cap;
    }
}

/* one‑time initialization of the smooth‑number generator */
static void init(void)
{
    if (smooth) return;                      /* already initialized */
    smooth_cap = 16;
    smooth = (seq_t *)malloc(smooth_cap * sizeof(seq_t));
    if (!smooth) abort();
    smooth[0] = (seq_t)1;                    /* A051038(1) = 1 */
    smooth_len = 1;
    for (int i = 0; i < 5; ++i) ptrs[i] = 0;
}

/* generate values of A051038 up to the requested index (1‑based) */
static void ensure_b_up_to(idx_t need)
{
    init();
    while (smooth_len < need) {
        /* find the minimum candidate among all primes */
        seq_t candidate = smooth[ptrs[0]] * primes[0];
        for (int i = 1; i < 5; ++i) {
            seq_t v = smooth[ptrs[i]] * primes[i];
            if (v < candidate) candidate = v;
        }

        /* skip duplicates */
        if (candidate == smooth[smooth_len - 1]) {
            for (int i = 0; i < 5; ++i)
                if (smooth[ptrs[i]] * primes[i] == candidate) ++ptrs[i];
            continue;
        }

        grow_smooth();
        smooth[smooth_len++] = candidate;
        for (int i = 0; i < 5; ++i)
            if (smooth[ptrs[i]] * primes[i] == candidate) ++ptrs[i];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined below offset 1 */
    ensure_b_up_to(n);
    return smooth[n - 1] * (seq_t)2310;     /* a(n) = 2310 * A051038(n) */
}
