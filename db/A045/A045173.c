/*
 * A045173  Numbers whose base-5 representation contains no 0's and exactly
 *          one 2.
 * https://oeis.org/A045173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): the n‑th positive integer whose base‑5 representation
 * consists of the digits 1,2,3,4 (no 0’s) and contains exactly one digit 2.
 * The function unravels the index into the lexicographic ordering of all
 * admissible base‑5 strings.  The ordering used is the numeric order of the
 * resulting integers. */
#include <stdbool.h>

#define MAX_LEN 64   /* enough for any n that fits in seq_t */

seq_t a(idx_t n)
{
    if (n < 1)                /* sequence starts at offset 1 */
        return 0;

    /* -------- Find the length L of the base‑5 representation -------- */
    idx_t target = n - 1;      /* 0‑based index across all terms          */
    idx_t cum    = 0;          /* cumulative count of short strings       */
    idx_t L      = 1;          /* current length being considered         */
    idx_t pow3   = 1;          /* 3^(L‑1)                                 */

    /* Count of admissible strings of current length, L = L * 3^(L‑1) */
    while (true) {
        idx_t count = L * pow3;    /* count for this L */
        if (cum + count > target)
            break;
        cum += count;
        ++L;
        pow3 *= 3;                 /* next power of 3 for increased L */
    }
    /* Now cum holds the total strings of lengths < L.
       target - cum is the index among strings of length L.          */
    idx_t sub = target - cum;      /* 0‑based sub‑index for length L       */

    /* -------- Precompute powers of 3 up to L-1 (compact array) -------- */
    idx_t pow3_arr[MAX_LEN];
    pow3_arr[0] = 1;
    for (idx_t i = 1; i < L; ++i)
        pow3_arr[i] = pow3_arr[i - 1] * 3;

    /* -------- Unrank the sub‑index to get the base‑5 digits -------- */
    bool two_used = false;           /* whether the unique ‘2’ has been placed */
    seq_t val = 0;                   /* accumulating the resulting integer   */

    for (idx_t pos = 0; pos < L; ++pos) {
        idx_t remaining = L - pos - 1;          /* positions after current  */
        for (int d = 1; d <= 4; ++d) {
            idx_t count;
            if (two_used) {
                /* ‘2’ already placed → cannot place another */
                if (d == 2)
                    count = 0;
                else
                    count = pow3_arr[remaining];  /* all remaining digits 3 choices */
            } else { /* ‘2’ not yet used */
                if (d == 2) {
                    /* place the ‘2’ now, all later digits 3 choices each */
                    count = pow3_arr[remaining];
                } else { /* choose a digit 1,3,4 */
                    if (remaining == 0)         /* no place left for the required ‘2’ */
                        count = 0;
                    else
                        count = remaining * pow3_arr[remaining - 1];
                }
            }

            if (sub < count) {
                /* select this digit */
                val = val * 5 + (seq_t)d;
                if (d == 2)
                    two_used = true;
                break;
            }

            sub -= count; /* skip this block of possibilities */
        }
    }

    return val;
}
