/*
 * A052464  Fixed points for operation of repeatedly replacing a number with
 *          the sum of the fifth power of its digits.
 * https://oeis.org/A052464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: enumerate all numbers up to 9^5 · 6 and collect those
   equal to the sum of the fifth powers of their decimal digits.  The
   list is stored once and then indexed.  The sequence is finite:
   the largest possible candidate is 59049·6 = 354294. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static const seq_t digit5[10] = {
    0, 1, 32, 243, 1024, 3125, 7776, 16807, 32768, 59049
};

static seq_t *solutions = NULL;
static size_t solcnt = 0;
static size_t solcap = 0;
static bool initialized = false;

static void compute_solutions(void)
{
    if (initialized) return;
    initialized = true;

    /* upper bound for search: 9^5 * 6 = 354294 */
    const idx_t upper = (idx_t)59049u * 6u;

    /* start with a small capacity, it will grow as needed */
    solcap = 16;
    solutions = (seq_t*)malloc(sizeof(seq_t) * solcap);

    seq_t sum;
    for (idx_t num = 0; num <= upper; ++num) {
        if (num == 0) {
            sum = digit5[0];
        } else {
            sum = 0;
            idx_t tmp = num;
            while (tmp) {
                sum += digit5[tmp % 10];
                tmp /= 10;
            }
        }
        if (sum == (seq_t)num) {
            if (solcnt == solcap) {
                solcap <<= 1;
                solutions = (seq_t*)realloc(solutions, sizeof(seq_t) * solcap);
            }
            solutions[solcnt++] = (seq_t)num;
        }
    }
}

seq_t a(idx_t n)
{
    compute_solutions();

    /* n is 1-based; if out of bounds return 0 (undefined behaviour otherwise) */
    if (n == 0 || n > solcnt) return 0;
    return solutions[n - 1];
}
