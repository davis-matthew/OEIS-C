/*
 * A105573  Numbers m such that m-4 and m+4 have 4 prime factors.
 * https://oeis.org/A105573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bigomega_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers m such that bigomega_uint64(m-4) == 4 and bigomega_uint64(m+4) == 4 */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/bigomega_uint64.h"

static seq_t *solutions = NULL;
static idx_t sol_size  = 0;
static size_t sol_cap  = 0;
static idx_t current   = 1;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but guard against invalid input */

    while (sol_size < n) {
        if (current >= 5) {
            if (bigomega_uint64((uint64_t)(current - 4)) == (seq_t)4 &&
                bigomega_uint64((uint64_t)(current + 4)) == (seq_t)4) {

                if (sol_size == sol_cap) {
                    size_t new_cap = sol_cap ? sol_cap * 2 : 4;
                    seq_t *tmp = (seq_t *)realloc(solutions, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    solutions = tmp;
                    sol_cap = new_cap;
                }
                solutions[sol_size++] = (seq_t)current;
            }
        }
        ++current;
    }
    return solutions[n - 1];
}
