/*
 * A141375  Duplicate of A107008.
 * https://oeis.org/A141375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = the n‑th prime p with p ≡ 1 (mod 24)
 * Range:     i8 n <= 2, i16 n <= 45, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(1) = 73, a(2) = 97, a(3) = 193, …                 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* You may use this helper from the provided utilities: */
#include <math.h>

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* out of bounds, not used for OEIS offset 1 */

    /* Start with a rough upper bound for the n‑th prime (using the
       provided utility) and enlarge it until we find enough primes of
       the form 24k+1. */
    idx_t limit = _prime_limit_for_idx(n) * 4ULL;
    if (limit < 200ULL)
        limit = 200ULL;

    while (1) {
        /* Allocate the sieve array.  In C++ the cast is required. */
        bool *is_composite = (bool *)calloc((size_t)limit + 1, sizeof(bool));
        if (is_composite == NULL)
            return 0;            /* allocation failure */

        /* Sieve of Eratosthenes up to 'limit'. */
        for (idx_t p = 2; p * p <= limit; ++p) {
            if (!is_composite[p]) {
                for (idx_t m = p * p; m <= limit; m += p)
                    is_composite[m] = true;
            }
        }

        /* Count primes of the required congruence class. */
        idx_t count = 0;
        seq_t result = 0;
        for (idx_t x = 5; x <= limit; x += 2) {   /* skip even numbers */
            if (!is_composite[x] && (x % 24ULL) == 1ULL) {
                ++count;
                if (count == n) {
                    result = (seq_t)x;
                    break;
                }
            }
        }

        free(is_composite);

        if (count >= n)        /* found the n‑th term */
            return result;

        /* Not enough primes yet – increase the limit and try again. */
        limit *= 2ULL;
        if (limit == 0)     /* overflow protection, though unlikely */
            return 0;
    }
}
