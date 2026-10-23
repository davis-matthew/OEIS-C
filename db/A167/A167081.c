/*
 * A167081  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^13 = I.
 * https://oeis.org/A167081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=28, a(2)=756, a(3)=20412, a(4)=551124,
 *            a(5)=14880348, a(6)=401769396, a(7)=10847773692,
 *            a(8)=292889889684, a(9)=7908027021468, a(10)=213516729579636,
 *            a(11)=5764951698650172, a(12)=155653695863554644,
 *            a(13)=4202649788315975010. For n≥13 the sequence satisfies the
 *            linear recurrence a(n...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[14] = {
    (seq_t)1ULL,
    (seq_t)28ULL,
    (seq_t)756ULL,
    (seq_t)20412ULL,
    (seq_t)551124ULL,
    (seq_t)14880348ULL,
    (seq_t)401769396ULL,
    (seq_t)10847773692ULL,
    (seq_t)292889889684ULL,
    (seq_t)7908027021468ULL,
    (seq_t)213516729579636ULL,
    (seq_t)5764951698650172ULL,
    (seq_t)155653695863554644ULL,
    (seq_t)4202649788315975010ULL
};

static const seq_t COEF26  = (seq_t)26ULL;
static const seq_t COEF351 = (seq_t)351ULL;

/* Cached values of the sequence.  The array grows as needed. */
static seq_t *cache      = NULL;
static idx_t cache_len   = 0;   /* number of elements stored (≥14 after init) */

seq_t a(idx_t n)
{
    /* Initialise cache on first call. */
    if (cache == NULL) {
        cache_len = 14;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        for (idx_t i = 0; i < cache_len; ++i) {
            cache[i] = init_vals[i];
        }
    }

    /* Extend cache up to n if necessary. */
    while (cache_len <= n) {
        seq_t next = (seq_t)0;
        for (idx_t k = 1; k <= 12; ++k) {
            next += COEF26 * cache[cache_len - k];
        }
        next -= COEF351 * cache[cache_len - 13];

        /* Resize array if needed and store the new value. */
        idx_t new_len = cache_len + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) {
            abort();  /* Allocation failed */
        }
        cache = tmp;
        cache[cache_len] = next;
        cache_len = new_len;
    }

    return cache[n];
}
