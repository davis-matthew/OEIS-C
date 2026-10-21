/*
 * A077490  a(n) = smallest k such that 4k has a digit sum = n.
 * https://oeis.org/A077490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest k such that digit_sum(4*k) = n
 * Range:     u8 n <= 6, u16 n <= 27, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   ceil_div9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ceil_div9.h"

/* sequence types */
typedef seq_t seq_t;
typedef idx_t idx_t;

static const int pairs[25][2] = {
    {0,0},{0,4},{0,8},
    {1,2},{1,6},
    {2,0},{2,4},{2,8},
    {3,2},{3,6},
    {4,0},{4,4},{4,8},
    {5,2},{5,6},
    {6,0},{6,4},{6,8},
    {7,2},{7,6},
    {8,0},{8,4},{8,8},
    {9,2},{9,6}
};

seq_t a(idx_t n)
{
    /* special single‑digit cases */
    if (n == 4) return (seq_t)1;   /* 4*1 = 4, sum 4 */
    if (n == 8) return (seq_t)2;   /* 4*2 = 8, sum 8 */

    seq_t best_k = 0;
    int best_set = 0;

    /* iterate over all admissible last two digits */
    for (int t = 0; t < 25; ++t) {
        int a_dig = pairs[t][0];
        int b_dig = pairs[t][1];
        idx_t sum_ab = (idx_t)a_dig + (idx_t)b_dig;
        if (sum_ab > n) continue;

        idx_t r = n - sum_ab;                 /* remaining sum for prefix   */
        idx_t l = (r > 0) ? ceil_div9(r) : 0; /* number of prefix digits     */
        if (r > 0 && l == 0) continue;        /* impossible case */

        /* construct minimal prefix with digit sum = r and length l */
        seq_t prefix = 0;
        unsigned int remaining = (unsigned int)r; /* r is small (<= n <= few 10^4) */
        int num_digits = (int)l;
        for (int i = 0; i < num_digits; ++i) {
            int digits_left = num_digits - i - 1;
            int min_digit = (i == 0 && num_digits > 0) ? 1 : 0;
            int bound = remaining - 9 * digits_left;
            int digit = (bound > min_digit) ? bound : min_digit;
            if (digit > 9) digit = 9;
            prefix = prefix * (seq_t)10 + (seq_t)digit;
            remaining -= digit;
        }

        /* assemble full number m = prefix * 100 + a*10 + b */
        seq_t m = prefix * (seq_t)100 + (seq_t)a_dig * (seq_t)10 + (seq_t)b_dig;
        seq_t k = m / (seq_t)4;

        if (!best_set || k < best_k) {
            best_k = k;
            best_set = 1;
        }
    }

    return best_k;
}
