/*
 * A077139  a(1) = 1, a(n) = lcm(n, a(n-1)) / gcd(n, a(n-1)).
 * https://oeis.org/A077139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(n, a(n‑1)) / gcd(n, a(n‑1)) Equivalent recurrence:
 *            a(1)=1; for n>1, a(n)= (n/g) * (a(n‑1)/g) where g = gcd(n,
 *            a(n‑1))
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 31, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Greatest‑common divisor for generic seq_t type.
   Works for built‑in integer types as well as arbitrary‑precision classes
   that support the %= and == operators.  */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Cache for previously computed terms of the sequence.
   cache[0] is unused; cache[1] contains the first term. */
static seq_t *cache = NULL;
static idx_t cached_n = 0;   /* highest n already computed */

/* Compute the n‑th term of A077139.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* defined only for n>=1, but guard for safety */

    /* Ensure cache is large enough. */
    if (n > cached_n) {
        size_t needed = (size_t)(n + 1);
        if (cache == NULL) {
            cache = (seq_t *)malloc(needed * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        }

        /* Initialise the first term if this is the first call. */
        if (cached_n == 0) {
            cache[1] = (seq_t)1;
        }

        /* Generate all missing terms up to n. */
        for (idx_t i = cached_n + 1; i <= n; ++i) {
            if (i == 1) {
                cache[i] = (seq_t)1;
                continue;
            }
            seq_t prev = cache[i - 1];
            seq_t g    = seq_gcd(prev, (seq_t)i);   /* gcd(a(i‑1), i) */
            seq_t x    = (seq_t)i / g;             /* i / g   */
            seq_t y    = prev / g;                 /* a(i‑1) / g */
            cache[i]   = x * y;                    /* a(i) = x * y */
        }
        cached_n = n;
    }

    return cache[n];
}
