/*
 * A031182  Periods of sum of fourth powers of digits iterated until the
 *          sequence becomes periodic.
 * https://oeis.org/A031182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   sum_fourth_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A031182:
 * Periods of sum of fourth powers of digits iterated until the sequence
 * becomes periodic.  For a starting integer n, iterate
 *
 *      g(x) = sum of fourth powers of the decimal digits of x
 *
 * and let the sequence eventually enter a cycle.  The returned value
 * is the length of that cycle.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/sum_fourth_digits.h"

/* maximum value that can arise from a single iteration of the
 * digit‑power function for numbers that appear from starting
 * values up to 10 000.  It is safely larger than the theoretical
 * maximum 9^4 * 20 ≈ 131 220, and generous enough for larger
 * starting numbers used in the tests.
 */
#define MAXV 2000000
/* sufficient depth for the transient part of the sequence */
#define MAX_PATH 10000

/* static tables for memoisation and for detecting already visited
 * numbers during the current evaluation.
 */
static bool    _seq_init = false;
static int     _visited[MAXV];
static seq_t    _period[MAXV];

/* main sequence function */
seq_t a(idx_t n)
{
    /* initialise static arrays once */
    if (!_seq_init) {
        memset(_visited, 0xff, sizeof(_visited)); /* -1 */
        memset(_period, 0, sizeof(_period));
        _seq_init = true;
    }

    seq_t cur = (seq_t)n;
    seq_t path[MAX_PATH];
    size_t psz = 0;
    seq_t cycle_len = 0;

    /* evaluate until a cycle is found or a memoised period is hit */
    while (1) {
        if (cur < MAXV && _period[(size_t)cur] != 0) {
            /* recurring part already known */
            cycle_len = _period[(size_t)cur];
            break;
        }
        if (cur < MAXV && _visited[(size_t)cur] != -1) {
            /* first repetition: the cycle starts at visited[cur] */
            cycle_len = (seq_t)(psz - _visited[(size_t)cur]);
            break;
        }
        /* new state */
        if (cur < MAXV)
            _visited[(size_t)cur] = (int)psz;

        if (psz < MAX_PATH)
            path[psz++] = cur;
        else {
            /* if the transient part is unexpectedly long, grow array */
            /* realloc not used here: not expected for given constraints */
        }

        /* iterate */
        cur = sum_fourth_digits(cur);
    }

    /* memoise the cycle length for all visited states of this run */
    for (size_t i = 0; i < psz; ++i) {
        seq_t v = path[i];
        if (v < MAXV)
            _period[(size_t)v] = cycle_len;
    }

    /* clear visited markers */
    for (size_t i = 0; i < psz; ++i) {
        seq_t v = path[i];
        if (v < MAXV)
            _visited[(size_t)v] = -1;
    }

    return cycle_len;
}
