/*
 * A135235  Numbers k such that k = s^phi(s) where s is sum of its digits.
 * https://oeis.org/A135235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, nonn, bref
 * Formula:   a(n) = s^φ(s) where s = sum of decimal digits of a(n)
 * Range:     u8 none, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   big_pow, calc_phi, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/digit_sum.h"
#include "../utils/calc_phi.h"

/* Computes the n‑th term of OEIS A135235.  */
seq_t a(idx_t n) {
    /* static storage for the discovered terms */
    static seq_t solutions[8] = {0};
    static size_t solution_cnt = 0;
    static bool ready = false;

    if (!ready) {
        /* Search for all solutions with s up to a modest bound.
           All known solutions have s <= 400, and there are only four of them.
           The loop here will discover them all. */
        const idx_t MAX_S = 500;
        size_t i;
        for (idx_t s = 1; s <= MAX_S; ++s) {
            seq_t base = (seq_t)s;
            seq_t phi  = calc_phi(base);
            seq_t val  = big_pow(base, phi);
            seq_t sum  = digit_sum(val);
            if (sum == base) {
                /* store the found value */
                if (solution_cnt < 8) {
                    solutions[solution_cnt++] = val;
                }
            }
        }

        /* sort the solutions in increasing order (they are only a few) */
        for (i = 1; i < solution_cnt; ++i) {
            seq_t temp = solutions[i];
            size_t j = i;
            while (j > 0 && solutions[j-1] > temp) {
                solutions[j] = solutions[j-1];
                --j;
            }
            solutions[j] = temp;
        }

        ready = true;
    }

    if (n == 0 || (idx_t)n > solution_cnt)
        return (seq_t)0;   /* undefined for n beyond the known terms */

    return solutions[(size_t)n - 1];
}
