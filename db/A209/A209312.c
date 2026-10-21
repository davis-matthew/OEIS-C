/*
 * A209312  Number of practical numbers p<n with n-p and n+p both prime or
 *          both practical.
 * https://oeis.org/A209312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of practical numbers p < n such that * (n-p) and
 *            (n+p) are both prime or both practical
 * Range:     u8 n <= 286, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPractical, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "../utils/is_prime_small.h"
#include "../utils/isPractical.h"

/* Static cache of practical numbers */
static uint64_t *practical_list = NULL;   /* array of practical numbers */
static size_t practical_cnt   = 0;        /* number of elements in array   */
static size_t practical_cap   = 0;        /* allocated capacity             */
static size_t practical_max   = 0;        /* max number for which we have
                                           a practicality test               */

/* Ensure practical numbers up to limit-1 are generated and stored */
static void ensure_practicals_up_to(size_t limit)
{
    if (limit <= practical_max) return;   /* already done */

    for (size_t num = practical_max + 1; num <= limit; ++num) {
        if (isPractical(num)) {
            if (practical_cnt == practical_cap) {
                size_t new_cap = practical_cap ? (practical_cap << 1) : 16;
                uint64_t *new_arr = (uint64_t *)realloc(practical_list,
                                                        new_cap * sizeof(uint64_t));
                assert(new_arr);
                practical_list = new_arr;
                practical_cap = new_cap;
            }
            practical_list[practical_cnt++] = num;
        }
    }
    practical_max = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* a(1)=0, a(2)=0 from the sequence definition */

    /* Build practical numbers up to n-1 once */
    ensure_practicals_up_to((size_t)n - 1);

    seq_t count = 0;

    for (size_t i = 0; i < practical_cnt; ++i) {
        uint64_t p = practical_list[i];
        if (p >= n) break;                  /* safety in case of mis‑run */

        uint64_t m1 = n - p;
        uint64_t m2 = n + p;

        bool both_prime = is_prime_small(m1) && is_prime_small(m2);
        bool both_practical = isPractical(m1) && isPractical(m2);

        if (both_prime || both_practical) ++count;
    }
    return count;
}
