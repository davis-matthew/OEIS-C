/*
 * A164778  Numbers n with property that average digit of n^2 is s=6.
 * https://oeis.org/A164778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 815, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A164778: numbers n whose average decimal digit of n^2 equals 6 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#ifdef __cplusplus
  #include <limits>
  #include <type_traits>
#endif

/* helper: count digits of x (x > 0) */
static unsigned digit_count_seq(seq_t x)
{
    unsigned len = 0;
    while (x > 0) {
        x /= 10;
        ++len;
    }
    return len ? len : 1;
}

/* helper: sum decimal digits of x (x > 0) */
static seq_t digit_sum_seq(seq_t x)
{
    seq_t sum = 0;
    while (x > 0) {
        sum += x % 10;
        x /= 10;
    }
    return sum;
}

/* cache of known terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 3;   /* first multiple of 3 after 24 */

seq_t a(idx_t n)
{
    /* Populate cache until we have at least n terms. */
    while (cache_len < (size_t)n) {
        seq_t cand = (seq_t)next_candidate;

        /* Detect overflow of cand*cand for integral seq_t. */
#if defined(__cplusplus)
        if (std::is_integral<seq_t>::value) {
            const seq_t max_val = std::numeric_limits<seq_t>::max();
            if (cand > max_val / cand) {
                /* further candidates will also overflow */
                break;
            }
        }
#else
        if (cand > (seq_t)(-1) / cand) {
            break;
        }
#endif
        seq_t sq = cand * cand;
        unsigned len = digit_count_seq(sq);
        seq_t sum = digit_sum_seq(sq);

        if (sum == (seq_t)6 * (seq_t)len) {
            /* Grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
                if (!new_arr) return 0;   /* allocation failure */
                for (size_t i = 0; i < cache_len; ++i) {
                    new_arr[i] = cache[i];
                }
                free(cache);
                cache = new_arr;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }

        next_candidate += 3;
    }

    if (cache_len < (size_t)n) {
        return 0;  /* undefined for this n under current build */
    }
    return cache[n - 1];
}
