/*
 * A069532  Smallest even number with digit sum n.
 * https://oeis.org/A069532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 40, u32 n <= 83, u64 n <= 170,
 *            u128 n <= 343, big any n
 * Helpers:   feasible
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest even number with digit sum n.
 *  The sequence is defined by the property that the digit sum of the
 *  returned integer equals n and the integer is even.
 *  The algorithm constructs the number digit by digit, trying the
 *  minimal possible length and selecting digits in increasing order
 *  to guarantee the smallest value.  It uses a depth‑first search
 *  with pruning (feasible()) to avoid impossible branches.  The
 *  result is built incrementally using only +, * and assignment,
 *  so it works for all supported seq_t types, including arbitrary‑precision
 *  integers.  Complexity is O(n) in the length of the result
 *  (≈ n/9 digits) which is well below the time limit for n up to 10000.  */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/feasible.h"

/* DFS to build the digits of the desired number.
 * pos : current position (0 … L-1)
 * rem : remaining digit sum to distribute
 * L   : total length
 * digits : buffer that will contain the chosen digits on success */
static bool dfs_rec(unsigned int pos,
                    unsigned long long rem,
                    unsigned int L,
                    unsigned char *digits)
{
    if (pos == L)                /* all positions decided */
        return rem == 0;         /* success only if sum exhausted */

    unsigned int min_d = (pos == 0) ? 1 : 0;   /* first digit cannot be 0 */
    unsigned int R = L - pos - 1;              /* digits to fill after this one */

    for (unsigned int d = min_d; d <= 9; ++d) {
        if (rem < d) break;                 /* further digits would be too large */

        unsigned long long rem_after = rem - d;
        if (rem_after > 9ULL * R) continue; /* not enough capacity for the tail */

        if (R == 0) {                       /* last digit: must be even */
            if (rem_after != 0) continue;
            if (d % 2 != 0) continue;
            digits[pos] = (unsigned char)d;
            return true;
        }

        /* prune using the feasibility test */
        if (!feasible(rem_after, R)) continue;

        digits[pos] = (unsigned char)d;
        if (dfs_rec(pos + 1, rem_after, L, digits))
            return true;
    }
    return false;                         /* no digit works here */
}

/* Public entry point for the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) is not defined in OEIS; return 0 safely */
        return (seq_t)0;

    unsigned int min_len = (unsigned int)((n + 8ULL) / 9ULL);   /* ceil(n/9) */

    for (unsigned int L = min_len; ; ++L) {
        /* allocate temporary buffer for the digits */
        unsigned char *digits = (unsigned char*)malloc(L);
        if (!digits) return (seq_t)0;   /* unlikely failure */

        if (dfs_rec(0, (unsigned long long)n, L, digits)) {
            /* build the value from the digits */
            seq_t val = 0;
            for (unsigned int i = 0; i < L; ++i)
                val = val * 10 + (seq_t)digits[i];

            free(digits);
            return val;
        }
        free(digits);

        /* it is almost impossible that we need many more digits; guard */
        if (L > min_len + 50)
            return (seq_t)0;
    }
}
