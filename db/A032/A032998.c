/*
 * A032998  If d,e are consecutive digits of n in base 10, then |d-e|>=5.
 * https://oeis.org/A032998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) by brute‑force search: iterate natural numbers and select
 * those whose consecutive decimal digits differ by at least 5.
 * The function returns the nth such positive integer (offset 1). */

#include <stdbool.h>

static inline bool good(seq_t x)
{
    int last = -1;
    while (x > 0) {
        int d = (int)(x % 10);           /* current least‑significant digit   */
        if (last != -1) {
            int diff = last - d;
            if (diff < 0) diff = -diff;  /* |last-d|                            */
            if (diff < 5) return false;  /* violates the condition              */
        }
        last = d;
        x /= 10;                          /* remove least‑significant digit      */
    }
    return true;
}

seq_t a(idx_t n)
{
    seq_t count = 0;     /* number of qualifying terms found so far */
    seq_t cand   = 0;     /* candidate integer to test                 */

    while (1) {
        ++cand;          /* iterate through natural numbers           */
        if (good(cand)) { /* test the digit‑difference property      */
            ++count;
            if (count == n) return cand; /* nth qualifying term found */
        }
    }
}
