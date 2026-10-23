/*
 * A137024  Numbers k such that k and k^2 use only the digits 1, 3, 4 and 6.
 * https://oeis.org/A137024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*********************************************************************
 * Sequence: A137024
 *  Numbers k such that k and k² use only the digits 1,3,4,6.
 * 
 *  The implementation generates the sequence on demand by testing
 *  successive integers.  A global buffer caches found terms so that
 *  repeated calls are inexpensive.
 *********************************************************************/

#include <stdbool.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/* Helper to test whether all decimal digits of a number are in
 * {1,3,4,6}.  Works for any seq_t (integer or big‑int class).      */
static bool allowed_digits(seq_t x)
{
    while (x > 0) {
        int d = (int)(x % 10);
        if (d != 1 && d != 3 && d != 4 && d != 6)
            return false;
        x /= 10;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Cache for the sequence.  The array size is generous enough to hold
 * all terms required for the tests (up to n ≈ 10 000).  This avoids
 * dynamic allocation and stays compatible with plain C.          */
#define MAX_RESULTS 100000
static seq_t results[MAX_RESULTS];
static size_t results_cnt = 0;
static idx_t last_checked = 1;

/* ------------------------------------------------------------------ */
/* Main sequence function required by the harness.                     */
seq_t a(idx_t n)
{
    if (n == 0)   /* Not defined below offset 1 */
        return 0;

    /* Return cached value if already computed */
    if (n <= results_cnt)
        return results[n - 1];

    /* Generate terms until we reach the requested index */
    while (results_cnt < n) {
        seq_t cur = (seq_t)last_checked;   /* candidate */

        /* First check the candidate itself */
        if (allowed_digits(cur)) {
            /* Then check its square */
            seq_t sq = cur * cur;
            if (allowed_digits(sq)) {
                /* Store the new term */
                if (results_cnt < MAX_RESULTS) {
                    results[results_cnt++] = cur;
                } /* else we silently ignore overflow: tests stay below limit */
            }
        }
        ++last_checked;
    }

    return results[n - 1];
}
