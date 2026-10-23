/*
 * A174236  n-th number k such that k*4-+1 is twin prime pair plus n-th number
 *          m such that m*4+2-+1 is twin prime pair.
 * https://oeis.org/A174236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A045753(n) + A123986(n)
 * Range:     u8 n <= 13, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A045753(n) = n such that 4n-1 and 4n+1 are both prime   */
/*  A123986(n) = n such that 4n+1 and 4n+3 are both prime   */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */

seq_t a(idx_t n)
{
    /* Static caches for the two subsequences */
    static seq_t  *list1 = NULL;   /* A045753 values */
    static seq_t  *list2 = NULL;   /* A123986 values */
    static idx_t   list1_size = 0, list2_size = 0;
    static idx_t   list1_cap = 0, list2_cap = 0;
    static idx_t   next_k1 = 1;     /* next candidate k for list1   */
    static idx_t   next_k2 = 1;     /* next candidate k for list2   */

    /* Initial allocation on the first call */
    if (list1 == NULL) {
        const idx_t init_cap = 2000; /* enough for small n, will grow as needed */
        list1 = (seq_t *)malloc(init_cap * sizeof(seq_t));
        list2 = (seq_t *)malloc(init_cap * sizeof(seq_t));
        list1_cap = list2_cap = init_cap;
    }

    /* Build list1 up to the required index n */
    while (list1_size < n) {
        idx_t k = next_k1++;
        idx_t p1 = 4ULL * k - 1;
        idx_t p2 = 4ULL * k + 1;
        if (is_prime(p1) && is_prime(p2)) {
            if (list1_size == list1_cap) {
                list1_cap <<= 1;
                list1 = (seq_t *)realloc(list1, list1_cap * sizeof(seq_t));
            }
            list1[list1_size++] = (seq_t)k;
        }
    }

    /* Build list2 up to the required index n */
    while (list2_size < n) {
        idx_t k = next_k2++;
        idx_t p1 = 4ULL * k + 1;
        idx_t p2 = 4ULL * k + 3;
        if (is_prime(p1) && is_prime(p2)) {
            if (list2_size == list2_cap) {
                list2_cap <<= 1;
                list2 = (seq_t *)realloc(list2, list2_cap * sizeof(seq_t));
            }
            list2[list2_size++] = (seq_t)k;
        }
    }

    /* Return the sum of the n‑th terms of both sequences */
    return list1[n - 1] + list2[n - 1];
}
