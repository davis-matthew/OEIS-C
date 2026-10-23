/*
 * A046711  From the Bruck-Ryser theorem: numbers n == 1 or 2 (mod 4) which
 *          are also the sum of 2 squares.
 * https://oeis.org/A046711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A046711: numbers n ≡ 1 or 2 (mod 4) that are the sum of two squares.
   The function accepts an index n (1‑based) and returns the n‑th such number.
   It builds the sequence on demand, caching all values for reuse. */

/*  All helper functions below are written in C and are
    compatible with both the pure integer and arbitrary‑precision builds. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Test whether a positive integer m is expressible as a sum of two squares.
   By Fermat's theorem on sums of two squares a number is representable iff
   every prime factor ≡ 3 (mod 4) occurs to an even power.  The factor 2
   itself never affects the condition. */
static bool is_sum_of_two_squares(idx_t m)
{
    if (m == 0) return true;          /* 0 = 0² + 0², not used but safe */

    idx_t t = m;

    /* Remove all factors of 2 – they are irrelevant for the test. */
    while ((t & 1) == 0) t >>= 1;     /* t /= 2 */

    for (idx_t p = 3; p <= t / p; p += 2) {
        if (t % p == 0) {
            int odd = 0;             /* track parity of exponent */
            do {
                t /= p;
                odd ^= 1;
            } while (t % p == 0);
            if ((p & 3) == 3 && odd) return false;
        }
    }

    /* Any remaining factor > 1 is prime. */
    if (t > 1 && (t & 3) == 3) return false;
    return true;
}

/* Calculation of the n-th term of A046711, 1‑based index. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* defined for completeness */

    /* Static state for memoisation. */
    static idx_t *cache = NULL;      /* stores terms as idx_t */
    static size_t len   = 0;         /* number of terms currently cached */
    static size_t cap   = 0;         /* allocated capacity of cache */
    static idx_t tested = 0;         /* last candidate examined */

    /* Expand cache until it contains at least n terms. */
    while (len < (size_t)n) {
        /* Find the next candidate congruent to 1 or 2 modulo 4. */
        do { ++tested; } while ((tested & 3) != 1 && (tested & 3) != 2);

        if (is_sum_of_two_squares(tested)) {
            if (len == cap) {                 /* grow cache */
                size_t new_cap = cap ? cap * 2 : 8;
                idx_t *tmp = (idx_t *)realloc(cache, new_cap * sizeof(idx_t));
                if (!tmp) abort();            /* allocation failure */
                cache = tmp;
                cap   = new_cap;
            }
            cache[len++] = tested;
        }
    }

    return (seq_t)cache[n - 1];
}
