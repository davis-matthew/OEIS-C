/*
 * A055926  Numbers k such that {largest m such that 1, 2, ..., m divide k} is
 *          different from {largest m such that m! divides k}; numbers k which
 *          are either odd multiples of 12 or the largest m such that (m-1)!
 *          divides k is a composite number > 5.
 * https://oeis.org/A055926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 2724, u32 n <= 9528, u64 n <= 9528,
 *            u128 n <= 9528, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A055926 by scanning integers and testing if the largest m
   with lcm(1..m) | k is greater than the largest m with m! | k. */

#include <stdbool.h>
#include "../utils/gcd.h"

#define MAX_M 20

static seq_t lcm_arr[MAX_M + 1];
static seq_t fact_arr[MAX_M + 1];
static bool init_done = false;

/* Initialise lcm and factorial arrays once. */
static void init_arrays(void) {
    if (init_done) return;
    init_done = true;
    lcm_arr[0] = 1;
    fact_arr[0] = 1;
    for (idx_t m = 1; m <= MAX_M; ++m) {
        seq_t prev = lcm_arr[m - 1];
        seq_t g = gcd(prev, (seq_t)m);
        seq_t div = prev / g;
        lcm_arr[m] = div * (seq_t)m;
        fact_arr[m] = fact_arr[m - 1] * (seq_t)m;
    }
}

seq_t a(idx_t n) {
    init_arrays();
    idx_t count = 0;
    seq_t k = 1;

    while (true) {
        seq_t km = k;

        /* largest m with lcm(1..m) | k */
        idx_t f = 0;
        for (idx_t m = 1; m <= MAX_M && lcm_arr[m] <= km; ++m) {
            if (km % lcm_arr[m] == 0) f = m;
        }

        /* largest m with m! | k */
        idx_t g = 0;
        for (idx_t m = 1; m <= MAX_M && fact_arr[m] <= km; ++m) {
            if (km % fact_arr[m] == 0) g = m;
        }

        if (f > g) {
            ++count;
            if (count == n) return km;
        }
        ++k;
    }
}
