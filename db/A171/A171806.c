/*
 * A171806  Number of 5 X 5 permutation matrices such that the n-th matrix
 *          power is the least nonnegative power that gives the identity
 *          matrix.
 * https://oeis.org/A171806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of permutations in S_5 of order n
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   count_type, fact_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_type.h"
#include "../utils/fact_small.h"

/* helper: gcd for idx_t */
static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* helper: compute lcm for idx_t */
static idx_t lcm_idx(idx_t a, idx_t b)
{
    return (a / gcd_idx(a, b)) * b;
}

/* helper: compute order (lcm of cycle lengths) for a cycle type */
static idx_t order_type(const idx_t cycle[5])
{
    idx_t lcm = 1;
    for (int i = 0; i < 5; ++i) {
        if (cycle[i] > 0) {
            lcm = lcm_idx(lcm, cycle[i]);
        }
    }
    return lcm;
}

seq_t a(idx_t n)
{
    /* static initialization of order counts */
    static seq_t order_counts[7] = {0}; /* index by order 0..6 */
    static bool initialized = false;

    if (!initialized) {
        /* list of all cycle types of size 5 (up to 5 cycles) */
        const idx_t cycle_types[7][5] = {
            {1,1,1,1,1}, /* identity */
            {2,1,1,1},   /* one 2-cycle */
            {2,2,1},     /* two 2-cycles */
            {3,1,1},     /* one 3-cycle */
            {3,2},       /* 3-cycle + 2-cycle */
            {4,1},       /* one 4-cycle */
            {5}          /* one 5-cycle */
        };

        /* compute counts and orders for each type, accumulate per order */
        for (int i = 0; i < 7; ++i) {
            idx_t ord = order_type(cycle_types[i]);
            seq_t cnt = count_type(cycle_types[i]);

            if (ord <= 6) {
                order_counts[ord] += cnt;
            }
        }
        initialized = true;
    }

    if (n <= 6) {
        return order_counts[n];
    }
    return 0;
}
