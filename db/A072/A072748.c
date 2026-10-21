/*
 * A072748  Counting factor 3 in the first n squarefree numbers.
 * https://oeis.org/A072748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1022, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: iterate all integers from 1 upwards, test each for square‑freeness
 * (no repeated prime factor), and count how many of the first n such numbers
 * are divisible by 3.  The test for square‑freeness is a simple trial division
 * by squares: for each integer p with p*p <= x, if x is divisible by p*p
 * then x is not square‑free.  This runs in O(X * sqrt(X)) time, which
 * is comfortably fast for n up to about 10 000 (the 10 000‑th square‑free
 * number is ≈ 16 500).  No pre‑computed tables or memoisation are needed,
 * but the function works correctly regardless of call order and returns
 * the exact value for any n ≥ 1. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t count = 0;              /* number of divisor‑by‑3 square‑free seen */
    idx_t sf_seen = 0;            /* how many square‑free numbers have been found */
    idx_t x = 1;                  /* current integer to test */

    while (1) {
        bool squarefree = true;

        /* trial division by squares */
        for (idx_t p = 2; p * p <= x; ++p) {
            idx_t sq = p * p;
            if (x % sq == 0) {
                squarefree = false;
                break;
            }
        }

        if (squarefree) {
            ++sf_seen;
            if ((x % 3) == 0)
                ++count;
            if (sf_seen == n)
                return count;
        }
        ++x;
    }
}
