/*
 * A111653  n-th composite number appears n times.
 * https://oeis.org/A111653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, less, nonn
 * Formula:   a(n) = C_m, where m is the smallest integer with m*(m+1)/2 ≥ n,
 *            and C_m is the m‑th composite number
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* static state for memoisation */
static seq_t *composites = NULL;   /* array of composites (0‑based) */
static size_t comp_cap   = 0;      /* current allocation capacity */
static idx_t   comp_size  = 0;      /* number of composites stored */
static uint64_t next_test = 4;     /* next integer to test for compositeness */

/* ensure composites array can hold at least 'cap' elements */
static void ensure_comp_capacity(size_t cap)
{
    if (cap <= comp_cap)
        return;
    size_t new_cap = comp_cap ? comp_cap * 2 : 16;
    if (new_cap < cap) new_cap = cap;
    composites = (seq_t *)realloc(composites, new_cap * sizeof(seq_t));
    comp_cap = new_cap;
}

/* generate composites up to index 'm' (1‑based) */
static void generate_composites(idx_t m)
{
    while (comp_size < m) {
        /* find next composite */
        while (is_prime(next_test))
            ++next_test;
        /* store it */
        ensure_comp_capacity((size_t)(comp_size + 1));
        composites[comp_size++] = (seq_t)next_test;
        ++next_test;
    }
}

seq_t a(idx_t n)
{
    /* find minimal m such that m*(m+1)/2 ≥ n */
    idx_t m = 0;
    idx_t sum = 0;
    while (sum < n) {
        ++m;
        sum += m;
    }

    generate_composites(m);
    /* composites array is 0‑based, so return C_m = composites[m-1] */
    return composites[m - 1];
}
