/*
 * A178110  Consider the set of divisors d of binomial(n-d-1,d-1) where
 *          gcd(n,d)>1 and 1<d<=n/2. If the smallest element of this set is 6,
 *          n is in the sequence.
 * https://oeis.org/A178110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer n such that * the smallest
 *            divisor d of binomial(n-d-1, d-1) with * 1 < d ≤ n/2 and
 *            gcd(n,d) > 1 is exactly 6. * (If no such d exists the term is
 *            not included.)
 * Range:     u8 n <= 60, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation checks only d = 2,3,4,5,6 because any smaller
 * admissible divisor would immediately exclude n from the sequence,
 * and 6 is the desired smallest admissible divisor.  All arithmetic
 * for binomial exponents is done with Legendre's theorem.  The
 * sequence is generated on demand and memoised. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static uint64_t v_factorial(uint64_t n, uint64_t p)
{
    uint64_t v = 0;
    while (n) {
        n /= p;
        v += n;
    }
    return v;
}

/* Does d (2 ≤ d ≤ 6) divide binomial(n-d-1, d-1) ? */
static bool divides_binom(uint64_t n, int d)
{
    uint64_t a = n - (uint64_t)d - 1;   /* n - d - 1   */
    uint64_t b = (uint64_t)d - 1;       /* d - 1       */

    if (a < b) return false; /* binomial is zero */

    /* prime exponents needed for each d */
    uint64_t exp2 = 0, exp3 = 0, exp5 = 0;
    /* compute v_p(binomial(a,b)) */
    exp2 = v_factorial(a, 2) - v_factorial(b, 2) - v_factorial(a - b, 2);
    exp3 = v_factorial(a, 3) - v_factorial(b, 3) - v_factorial(a - b, 3);
    exp5 = v_factorial(a, 5) - v_factorial(b, 5) - v_factorial(a - b, 5);

    switch (d) {
    case 2:  return exp2 >= 1;
    case 3:  return exp3 >= 1;
    case 4:  return exp2 >= 2;
    case 5:  return exp5 >= 1;
    case 6:  return (exp2 >= 1) && (exp3 >= 1);
    default: return false;
    }
}

/* Return true iff n is in the sequence. */
static bool is_good(uint64_t n)
{
    if (n < 12u) return false;          /* need at least d = 6 ≤ n/2 */

    /* test d = 2,3,4,5: any admissible one excludes n */
    for (int d = 2; d <= 5; ++d) {
        if (gcd((seq_t)n, (seq_t)d) == 1) continue;   /* gcd(n,d)==1 */
        if (divides_binom(n, d)) return false;
    }

    /* d = 6 must be admissible */
    if (gcd((seq_t)n, (seq_t)6) == 1) return false;
    return divides_binom(n, 6);
}

/* Public function required by the hosting framework. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;
    static uint64_t current   = 2;      /* first candidate to test */

    if (!cache) {
        cache_cap = 256;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed */
    }

    while (cache_size < (size_t)n) {
        while (!is_good(current)) ++current;

        if (cache_size == cache_cap) {
            cache_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!tmp) { free(cache); cache = NULL; return (seq_t)0; }
            cache = tmp;
        }
        cache[cache_size++] = (seq_t)current;
        ++current;
    }

    return cache[n - 1];
}
