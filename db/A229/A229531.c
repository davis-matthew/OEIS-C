/*
 * A229531  Number of defective 3-colorings of an n X 5 0..2 array connected
 *          horizontally, diagonally and antidiagonally with exactly one
 *          mistake, and colors introduced in row-major 0..2 order.
 * https://oeis.org/A229531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 28*a(n-1) - 286*a(n-2) + 1290*a(n-3) - 2373*a(n-4) +
 *            304*a(n-5) + 3551*a(n-6) - 2846*a(n-7) - 546*a(n-8) +
 *            1308*a(n-9) - 505*a(n-10) + 76*a(n-11) - 4*a(n-12) Initial
 *            values: a(1)=16, a(2)=112, a(3)=1588, a(4)=19276, a(5)=221096,
 *            a(6)=2451728, a(7)=26566266, a(8)=283010776, a(9)=2975590424,...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#if defined(__cplusplus)
#include <vector>
#endif

/* Initial values, index 0 unused */
static const seq_t init_vals[14] = {
    0ULL,
    16ULL, 112ULL, 1588ULL, 19276ULL, 221096ULL, 2451728ULL,
    26566266ULL, 283010776ULL, 2975590424ULL, 30959068528ULL,
    319352065652ULL, 3270635802440ULL, 33292532186562ULL
};

#if defined(__cplusplus)
/* C++ implementation using std::vector */
static std::vector<seq_t> cache_vec;
static idx_t computed_up_to = 0;

static inline seq_t get_cache(idx_t i) {
    return cache_vec[(size_t)i];
}
static inline void set_cache(idx_t i, seq_t val) {
    if (cache_vec.size() <= (size_t)i)
        cache_vec.resize((size_t)i + 1);
    cache_vec[(size_t)i] = val;
}
#else
/* C implementation using malloc/realloc */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static idx_t computed_up_to = 0;

static void ensure_cache_size(idx_t n) {
    if (cache == NULL) {
        cache_size = n + 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
    } else if (cache_size <= (size_t)n + 1) {
        size_t new_size = cache_size * 2;
        if (new_size <= (size_t)n + 1)
            new_size = n + 2;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        cache_size = new_size;
    }
}

static inline seq_t get_cache(idx_t i) {
    return cache[(size_t)i];
}
static inline void set_cache(idx_t i, seq_t val) {
    if (cache == NULL || cache_size <= (size_t)i)
        ensure_cache_size(i);
    cache[(size_t)i] = val;
}
#endif

/* Initialise first 13 terms if not already done */
static void init_cache(void) {
    if (computed_up_to != 0)  /* already initialised */
        return;
    for (idx_t i = 1; i <= 13; ++i)
        set_cache(i, init_vals[i]);
    computed_up_to = 13;
}

seq_t a(idx_t n) {
    if (n == 0)  /* behaviour below offset is undefined, return 0 */
        return (seq_t)0;

    init_cache();  /* ensure the first 13 terms are ready */

    if (n <= computed_up_to)
        return get_cache(n);

    /* compute further terms up to n */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 13) {   /* safety, but will not happen */
            val = init_vals[i];
        } else {
            /* recurrence: positive and negative terms separated to avoid
               unsigned underflow during subtraction. */
            seq_t acc = 0;
            acc += (seq_t)28   * get_cache(i - 1);
            acc += (seq_t)1290 * get_cache(i - 3);
            acc += (seq_t)304  * get_cache(i - 5);
            acc += (seq_t)3551 * get_cache(i - 6);
            acc += (seq_t)1308 * get_cache(i - 9);
            acc += (seq_t)76   * get_cache(i - 11);
            acc -= (seq_t)286  * get_cache(i - 2);
            acc -= (seq_t)2373 * get_cache(i - 4);
            acc -= (seq_t)2846 * get_cache(i - 7);
            acc -= (seq_t)546  * get_cache(i - 8);
            acc -= (seq_t)505  * get_cache(i - 10);
            acc -= (seq_t)4    * get_cache(i - 12);
            val = acc;
        }
        set_cache(i, val);
    }
    computed_up_to = n;
    return get_cache(n);
}
