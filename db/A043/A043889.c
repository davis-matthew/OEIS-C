/*
 * A043889  Numbers k such that 0 and 4 occur juxtaposed in the base-5
 *          representation of k but not of k+1.
 * https://oeis.org/A043889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A043889: numbers k such that the base‑5 representation of k
   contains the adjacent pair 04 or 40, but the representation of k+1
   contains neither.  The function performs a brute‑force search over
   integers, checking the condition for each candidate until the nth term
   is found. */
#include <stdbool.h>
#include <stdint.h>

static inline bool has_adj_04_or_40(seq_t v)
{
    int prev = -1;                    /* previous digit (none yet) */
    while (v > 0) {
        int d = (int)(v % 5);         /* current least‑significant digit */
        if ((prev == 0 && d == 4) || (prev == 4 && d == 0))
            return true;              /* 04 or 40 found */
        prev = d;
        v /= 5;                       /* shift right one base‑5 digit */
    }
    return false;                     /* no pair found in this number */
}

seq_t a(idx_t n)
{
    seq_t count = 0;                   /* how many qualifying numbers seen */
    seq_t k = 0;                       /* current integer being tested */

    while (1) {
        if (has_adj_04_or_40(k) && !has_adj_04_or_40(k + 1)) {
            count++;
            if (count == (seq_t)n)     /* nth element reached */
                return k;
        }
        k++;                           /* next integer to test */
    }
}
