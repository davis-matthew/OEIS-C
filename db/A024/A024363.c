/*
 * A024363  Number of primitive Pythagorean triangles with side n.
 * https://oeis.org/A024363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2890, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = A024361(n)+A024362(n) for OEIS A024363:
   number of primitive Pythagorean triangles with side n. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 2) return 0;

    if ((n & 1) == 0) {                     /* even side: n = 2xy  (primitive leg) */
        idx_t d = n / 2;
        seq_t count = 0;
        for (idx_t y = 1; y * y <= d; ++y) {
            if (d % y) continue;
            idx_t x = d / y;
            if (x <= y) continue;          /* require x > y */
            if (gcd((seq_t)x, (seq_t)y) != 1) continue;
            if ((x & 1) == (y & 1)) continue; /* same parity ⇒ not primitive */
            ++count;
        }
        return count;
    } else {                                 /* odd side: could be an odd leg or hypotenuse */
        /* Count representations as odd leg: n = x² – y²  */
        seq_t odd_leg = 0;
        for (idx_t A = 1; A * A <= n; ++A) {
            if (n % A) continue;
            idx_t B = n / A;
            if (A >= B) continue;            /* require B > A */
            if (gcd((seq_t)A, (seq_t)B) != 1) continue;
            ++odd_leg;                       /* coprime odd factors give a primitive leg */
        }

        /* Count representations as hypotenuse: n = x² + y²  (primitive) */
        idx_t temp = n;
        int distinct_one_mod_four = 0;
        bool bad = false;
        for (idx_t p = 2; p * p <= temp; ++p) {
            if (temp % p == 0) {
                if (p % 4 == 3) { bad = true; break; } /* any 3 (mod 4) prime invalidates */
                while (temp % p == 0) temp /= p;       /* skip all powers */
                ++distinct_one_mod_four;               /* count distinct 1 (mod 4) primes */
            }
        }
        if (!bad && temp > 1) {
            if (temp % 4 == 3) bad = true;           /* remaining prime is 3 (mod 4) */
            else if (temp % 4 == 1) ++distinct_one_mod_four;
        }

        seq_t hyp = 0;
        if (!bad && distinct_one_mod_four > 0)
            hyp = ((seq_t)1 << (distinct_one_mod_four - 1));

        return odd_leg + hyp;
    }
}
