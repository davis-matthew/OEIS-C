/*
 * A223731  All positive numbers that are primitive sums of three nonzero
 *          squares.
 * https://oeis.org/A223731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer that is a primitive sum of three
 *            nonzero squares (a^2 + b^2 + c^2 with 0 < a ≤ b ≤ c,
 *            gcd(a,b,c)=1)
 * Range:     u8 n <= 33, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt, gcd, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/floor_sqrt.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n) {
    /* Static cache of computed sequence values */
    static seq_t  *seq   = NULL;
    static size_t  len   = 0;
    static size_t  cap   = 0;
    static idx_t   next_m = 1;   /* next integer to test */

    /* Ensure we have at least n terms computed */
    while (len < n) {
        idx_t m = next_m++;
        /* Quick Legendre test: skip numbers that cannot be
           expressed as a sum of 3 squares at all. */
        idx_t y = m;
        while ((y & 3) == 0) y >>= 2;   /* remove factors of 4 */
        if ((y & 7) == 7) continue;     /* form 4^k(8m+7) : impossible */

        /* Search for a primitive representation a^2 + b^2 + c^2 = m */
        bool found = false;
        for (idx_t a = 1; (a * a) < m && !found; ++a) {
            idx_t a2 = a * a;
            idx_t r  = m - a2;               /* remaining for b^2 + c^2 */

            for (idx_t b = a; (b * b) < r && !found; ++b) {
                idx_t b2 = b * b;
                idx_t c2 = r - b2;

                if (c2 < (b * b)) break;   /* c < b  → further b only larger */

                /* Check if c^2 is a perfect square */
                seq_t c = floor_sqrt((seq_t)c2);
                if ((seq_t)c * c == (seq_t)c2) {
                    /* Primitive check: gcd(a,b,c) == 1 */
                    seq_t g = gcd((seq_t)a, (seq_t)b);
                    g = gcd(g, (seq_t)c);
                    if (g == 1) {
                        found = true;
                        break;
                    }
                }
            }
        }

        if (found) {
            /* Expand cache if necessary */
            if (len == cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                seq = seq_realloc(seq, cap, new_cap);
                cap = new_cap;
            }
            seq[len++] = (seq_t)m;
        }
    }

    return seq[n - 1];
}
