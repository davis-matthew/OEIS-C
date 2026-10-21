/*
 * A087177  Number of even partition numbers <= P(n), where P=A000041.
 * https://oeis.org/A087177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of even partition numbers <= P(n), where P =
 *            partition numbers. We compute partition parity using the
 *            generalized pentagonal recurrence p(n) = Σ (-1)^{k-1} p(n -
 *            k(3k-1)/2) over k≠0. Since we only need parity, the signs become
 *            irrelevant and we XOR the contributions of the generaliz...
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static state: parity of partition numbers and cumulative even counts */
    static uint8_t *parity = NULL;      /* parity[i] == 0 -> even, 1 -> odd */
    static seq_t *cum_even = NULL;      /* cum_even[i] = count of even p(0..i) */
    static idx_t max_computed = 0;      /* largest n that has been computed */
    static idx_t capacity = 0;          /* allocated capacity for arrays */

    /* initialise static arrays on first call */
    if (parity == NULL) {
        capacity = 1;
        parity    = (uint8_t *)malloc(sizeof(uint8_t) * capacity);
        cum_even  = (seq_t *)malloc(sizeof(seq_t) * capacity);
        parity[0] = 1;                /* P(0) = 1 (odd) */
        cum_even[0] = 0;              /* no even partitions ≤ P(0) */
        max_computed = 0;
    }

    /* if more values requested, extend the tables */
    if (n > max_computed) {
        idx_t new_cap = n + 1;
        if (new_cap > capacity) {
            /* grow arrays to the required size */
            capacity = new_cap;
            parity   = (uint8_t *)realloc(parity, sizeof(uint8_t) * capacity);
            cum_even = (seq_t *)realloc(cum_even, sizeof(seq_t) * capacity);
        }
        /* compute new values */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            parity[i] = 0;  /* start with even */
            /* generalized pentagonal numbers:  k*(3k-1)/2 and k*(3k+1)/2 for k=1,2,... */
            for (idx_t k = 1; ; ++k) {
                /* pentagon for +k */
                unsigned __int128 tmp = (unsigned __int128)k * (3 * k - 1) / 2;
                if (tmp > i) break;
                idx_t pent1 = (idx_t)tmp;
                parity[i] ^= parity[i - pent1];

                /* pentagon for -k */
                tmp = (unsigned __int128)k * (3 * k + 1) / 2;
                if (tmp > i) break;
                idx_t pent2 = (idx_t)tmp;
                parity[i] ^= parity[i - pent2];
            }
            /* cumulative count of evens */
            cum_even[i] = cum_even[i - 1] + ((parity[i] == 0) ? 1 : 0);
        }
        max_computed = n;
    }
    return cum_even[(size_t)n];
}
