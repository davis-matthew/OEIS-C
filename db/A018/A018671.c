/*
 * A018671  Divisors of 832.
 * https://oeis.org/A018671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) enumerates the divisors of 832 in increasing order. */

#include <stdlib.h>
#include <stdbool.h>

/* compare function for qsort */
static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t v1 = *(const seq_t *)p1;
    const seq_t v2 = *(const seq_t *)p2;
    return (v1 < v2) ? -1 : (v1 > v2) ? 1 : 0;
}

/* static cache for the 14 divisors of 832 */
static seq_t divisors[15];
static size_t div_count = 0;
static bool divs_init = false;

seq_t a(idx_t n)
{
    if (!divs_init) {
        /* generate all divisors of 832 = 2^6 * 13^1 */
        size_t idx = 0;
        for (seq_t a_power = 1; a_power <= (seq_t)64; a_power <<= 1) { /* 2^a, a=0..6 */
            divisors[idx++] = a_power;          /* 13^0 * 2^a */
            divisors[idx++] = a_power * 13;     /* 13^1 * 2^a */
        }
        div_count = idx;                        /* should be 14 */
        qsort(divisors, div_count, sizeof(seq_t), cmp_seq);
        divs_init = true;
    }

    if (n == 0) return 0;                       /* undefined for n<1 */
    if ((idx_t)n > div_count) return 0;          /* beyond the finite list */
    return divisors[(idx_t)n - 1];
}
