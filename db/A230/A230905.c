/*
 * A230905  Number of n X 3 0..3 arrays x(i,j) with each element horizontally,
 *          vertically or antidiagonally next to at least one element with
 *          value (x(i,j)+1) mod 4, no adjacent elements equal, and upper left
 *          element zero.
 * https://oeis.org/A230905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 37*a(n-2) + 65*a(n-3) - 51*a(n-4) + 2*a(n-5)
 *            + 20*a(n-6) - 10*a(n-7) + 4*a(n-8) for n > 11 Initial values:
 *            a(1)=0 a(2)=8 a(3)=38 a(4)=152 a(5)=634 a(6)=2676 a(7)=11324
 *            a(8)=47954 a(9)=203122 a(10)=860468 a(11)=3645314
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* External types defined by the build system */
extern seq_t *alloc_seq_array(size_t n);
extern seq_t seq_from_str(const char *s);

/* Memoisation cache: cache[i] holds a(i) for i>=1 */
static seq_t *cache = NULL;        /* malloced array, index 0 unused */
static idx_t max_n = 0;            /* largest index already computed */
static size_t cap = 0;             /* allocated capacity */

/* Ensure the cache can hold at least indices 0..need */
static void ensure_capacity(idx_t need)
{
    if (cap >= (size_t)need + 1U)
        return;

    size_t new_cap = cap ? cap * 2 : 16;
    if (new_cap < (size_t)need + 1U)
        new_cap = need + 1U;

    /* realloc works with void* in both C and C++; cast explicitly */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* In the very unlikely event realloc fails, leave cache unchanged.
       For the contest environment memory limits are generous so we can
       ignore this case. */
    cache = tmp;
    cap = new_cap;
}

/* Initialise cache with known base values */
static void init_cache(void)
{
    if (cache != NULL)
        return;

    ensure_capacity(12); /* indices 0..11 */

    /* Index 0 is unused; 1‑based indices to match OEIS offset */
    cache[0] = 0;
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)8;
    cache[3] = (seq_t)38;
    cache[4] = (seq_t)152;
    cache[5] = (seq_t)634;
    cache[6] = (seq_t)2676;
    cache[7] = (seq_t)11324;
    cache[8] = (seq_t)47954;
    cache[9] = (seq_t)203122;
    cache[10] = (seq_t)860468;
    cache[11] = (seq_t)3645314;

    max_n = 11;
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    init_cache();

    if (n <= max_n)
        return cache[n];

    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t val = 0;

        /* Positive contributions */
        val += cache[i - 1] * (seq_t)10;
        val += cache[i - 3] * (seq_t)65;
        val += cache[i - 5] * (seq_t)2;
        val += cache[i - 6] * (seq_t)20;
        val += cache[i - 8] * (seq_t)4;

        /* Negative contributions */
        val -= cache[i - 2] * (seq_t)37;
        val -= cache[i - 4] * (seq_t)51;
        val -= cache[i - 7] * (seq_t)10;

        ensure_capacity(i);
        cache[i] = val;
    }

    max_n = n;
    return cache[n];
}
