/*
 * A162826  Positive numbers n such that 2*60*n/(60+n) are integers.
 * https://oeis.org/A162826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A162826: Positive numbers n such that 2*60*n/(60+n) is an integer.
   Equivalent condition: 60 + n divides 7200, i.e. let d = 60 + n, then
   d | 7200 and d > 60. Thus n = d - 60 where d runs over all divisors of 7200
   larger than 60. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;   /* array of all terms, sorted ascending */
static size_t cache_len = 0;  /* number of terms */

static void generate_cache(void)
{
    /* 7200 = 2^5 * 3^2 * 5^2, it has 54 divisors in total.
       Only those > 60 are needed. */
    seq_t tmp[54];
    size_t cnt = 0;

    for (idx_t d = 61; d <= 7200; ++d) {
        if (7200 % d == 0) {
            tmp[cnt++] = (seq_t)d - 60;   /* a term */
        }
    }

    /* Simple insertion sort – at most 30 elements */
    for (size_t i = 1; i < cnt; ++i) {
        seq_t key = tmp[i];
        size_t j = i;
        while (j > 0 && tmp[j - 1] > key) {
            tmp[j] = tmp[j - 1];
            --j;
        }
        tmp[j] = key;
    }

    cache = (seq_t *)malloc(cnt * sizeof(seq_t));
    for (size_t i = 0; i < cnt; ++i) {
        cache[i] = tmp[i];
    }
    cache_len = cnt;
}

seq_t a(idx_t n)
{
    if (cache == NULL) {
        generate_cache();
    }

    if (n == 0) return 0;          /* offset is 1, but guard anyway */

    if ((idx_t)(n - 1) < cache_len) {   /* valid index */
        return cache[(idx_t)(n - 1)];
    }

    /* For indices beyond the known terms, simply return 0 */
    return 0;
}
