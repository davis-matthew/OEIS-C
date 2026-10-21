/*
 * A216840  Smallest palindromic number of 3 digits in two bases differing by
 *          n.
 * https://oeis.org/A216840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 209, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A216840
 *
 *  a(n) = smallest integer that has a 3‑digit palindrome
 *  representation in base b and also in base b+n   (b>=2, n>=1)
 *
 *  The algorithm enumerates bases b in increasing order.
 *  For each base it generates all 3‑digit palindromes a d a,
 *  computes their value m, and checks whether the same m
 *  is a 3‑digit palindrome in base b+n.  The first value
 *  found for all bases that can possibly beat the current
 *  best is kept; the search stops when b²+1 is already larger
 *  than the best found.  Arithmetic is performed in 128‑bit
 *  integers to avoid overflow on builds where seq_t is
 *  smaller than 128 bits.
 */

#include <stdbool.h>
#include <stdint.h>

typedef unsigned __int128 u128;   /* internal type for safe arithmetic */

seq_t a(idx_t n)
{
    seq_t best = 0;      /* value of the current best candidate */
    bool   found = false;

    for (idx_t b = 2; ; ++b) {
        /* lower bound on 3‑digit palindromes in base b  */
        u128  b_big = (u128)b;
        u128  b_sq  = b_big * b_big;
        u128  minm  = b_sq + 1;        /* smallest possible m for this b */

        if (found && minm >= (u128)best)
            break;                    /* no larger base can improve best */

        idx_t base2 = b + n;
        u128  base2_big = (u128)base2;
        u128  base2_sq  = base2_big * base2_big;

        for (idx_t a_digit = 1; a_digit < b; ++a_digit) {
            u128 part_a = (u128)a_digit * (b_sq + 1);

            if (found && part_a > (u128)best)
                break;  /* larger a_digit would only increase m */

            for (idx_t d_digit = 0; d_digit < b; ++d_digit) {
                u128 m = part_a + (u128)d_digit * b_big;

                if (m < base2_sq) continue;               /* not 3‑digit in base b+n */

                u128 a2 = m / base2_sq;                   /* leading digit in base b+n */
                if (a2 == 0 || a2 >= (u128)base2) continue;

                u128 a0 = m % (u128)base2;                /* trailing digit in base b+n */
                if (a0 != a2) continue;                  /* not a palindrome */

                /* a valid candidate found */
                if (!found || m < (u128)best) {
                    best  = (seq_t)m;
                    found = true;
                }
                break;  /* increasing d_digit only increases m */
            }
        }
    }
    return best;
}
