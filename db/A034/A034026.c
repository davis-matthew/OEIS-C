/*
 * A034026  Numbers that are primitively or imprimitively represented by
 *          x^2+y^2, but not both.
 * https://oeis.org/A034026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force computation of A034026.
   For each candidate integer m we examine all representations m = a^2 + b^2,
   tracking whether a primitive (gcd(a,b)=1) or an imprimitive representation
   (gcd(a,b)>1) occurs.  The number m is accepted iff exactly one of these
   types occurs.  The sequence terms are stored in a static array so that
   calls with arbitrary indices are handled efficiently. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static inline seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static seq_t *cache = NULL;        /* stored sequence values */
static size_t cache_cap = 0;       /* current capacity of cache      */
static idx_t   terms_found = 0;    /* number of terms computed so far */
static seq_t   next_to_test = 1;   /* next integer to examine       */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined, keep type safety */

    /* Produce terms until we have at least n of them. */
    while (terms_found < n) {
        seq_t m = next_to_test;
        bool has_prim = false;
        bool has_imp  = false;

        seq_t limit = floor_sqrt(m);
        for (seq_t a = 0; a <= limit; ++a) {
            seq_t b2 = m - a * a;
            seq_t b  = floor_sqrt(b2);
            if (b * b != b2) continue;          /* not a perfect square */

            seq_t g = gcd_seq_t(a, b);
            if (g == 1) has_prim = true;
            else       has_imp  = true;

            if (has_prim && has_imp) break;     /* both types found */
        }

        if (has_prim ^ has_imp) {               /* accept m */
            /* Grow cache if necessary */
            if (!cache || terms_found == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *tmp = (seq_t *)realloc(cache,
                                               new_cap * sizeof(seq_t));
                if (!tmp) exit(1);              /* allocation error */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[terms_found++] = m;
        }
        ++next_to_test;
    }
    return cache[n - 1];
}
