/*
 * A014888  n is equal to the number of 4s in all numbers <= n written in base
 *          7.
 * https://oeis.org/A014888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = n such that total number of digit 4 in all base-7
 *            representations of 1..n equals n Brute-force enumeration of
 *            integers, storing solutions until the required index is reached
 * Range:     u8 none, u16 none, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *solutions = NULL;
static size_t sol_cap = 0;
static size_t sol_count = 0;
static idx_t last_i = 0;
static seq_t total_f = 0;

seq_t a(idx_t n)
{
    while (sol_count < n) {
        last_i++;
        idx_t x = last_i;
        int d = 0;
        while (x) {
            if ((x % 7ULL) == 4ULL)
                d++;
            x /= 7ULL;
        }
        total_f += (seq_t)d;
        if (total_f == (seq_t)last_i) {
            if (sol_count == sol_cap) {
                sol_cap = sol_cap ? sol_cap * 2 : 16;
                solutions = (seq_t *)realloc(solutions, sol_cap * sizeof(seq_t));
            }
            solutions[sol_count++] = (seq_t)last_i;
        }
    }
    return solutions[n-1];
}
