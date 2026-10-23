/*
 * A043604  Numbers whose base-5 representation has exactly 4 runs.
 * https://oeis.org/A043604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043604: Numbers whose base-5 representation has exactly 4 runs.
   The function enumerates integers in ascending order, testing the run count
   in base 5, and returns the (n‑1)‑th element of the resulting list.
*/

#include <stdlib.h>
#include <stdint.h>

/* Function supplied by the build environment */
extern uint64_t _floor_isqrt_u64(idx_t n);

seq_t a(idx_t n)
{
    /* Static data kept between calls */
    static seq_t *seq = NULL;          /* dynamic array of found terms */
    static size_t seq_len = 0;         /* number of terms stored */
    static size_t seq_cap = 0;         /* capacity of the array */
    static uint64_t last_checked = 0; /* next integer to examine */

    /* n is 1‑based; safety: cast to size_t for array indexing */
    const size_t target = (size_t)n;

    /* Keep generating terms until we have at least 'target' terms. */
    while (seq_len < target)
    {
        /* Scan successive integers until one with 4 runs is found. */
        while (1)
        {
            uint64_t val = last_checked;
            uint32_t runs = 0;      /* number of runs in current base‑5 number */
            int prev = -1;          /* previous digit, -1 means none yet */

            /* Count runs by examining base‑5 digits. */
            uint64_t tmp = val;
            while (tmp > 0)
            {
                int d = (int)(tmp % 5);
                if (prev == -1)        /* first digit */
                {
                    runs = 1;
                    prev = d;
                }
                else if (d != prev)    /* start of a new run */
                {
                    ++runs;
                    prev = d;
                }
                tmp /= 5;
            }
            /* If the number has exactly 4 runs, record it. */
            if (runs == 4)
            {
                if (seq_len + 1 > seq_cap)
                {
                    size_t new_cap = seq_cap ? seq_cap * 2 : 1024;
                    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                    seq_cap = new_cap;
                }
                seq[seq_len++] = (seq_t)val;
                break; /* move on to the next required term */
            }
            /* Advance to the next integer. */
            ++last_checked;
        }
        ++last_checked; /* skip the value just accepted for next search */
    }
    return seq[n - 1];
}
