/*
 * A064649  Row sums of the table A047916.
 * https://oeis.org/A064649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(n/d) * (n/d)^d * d!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Helper: integer power (seq_t base)^exp */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

/* Helper: φ(m) for uint64_t m, using trial division – fast enough for m ≤ 10⁴ */
static uint64_t phi_uint64(uint64_t m)
{
    uint64_t result = m;
    uint64_t p = 2;
    while (p * p <= m) {
        if (m % p == 0) {
            while (m % p == 0)
                m /= p;
            result -= result / p;
        }
        ++p;
    }
    if (m > 1)
        result -= result / m;
    return result;
}

/* Helper: factorial cache, grows as needed up to the largest d requested */
static seq_t *factorial_cache = NULL;
static idx_t factorial_cache_max = 0;   /* largest d for which factorial_cache[d] is valid */

static void ensure_factorial_cache(idx_t limit)
{
    if (limit <= factorial_cache_max)
        return;

    /* allocate new array of size limit+1 */
    seq_t *new_cache = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failed – aborting with zero (undefined behaviour in the build) */
        exit(EXIT_FAILURE);
    }

    /* copy old values */
    if (factorial_cache) {
        /* factorial_cache[0] is guaranteed to exist */
        for (idx_t i = 0; i <= factorial_cache_max; ++i)
            new_cache[i] = factorial_cache[i];
    } else {
        new_cache[0] = (seq_t)1;   /* 0! = 1 */
    }

    /* compute new entries */
    for (idx_t i = factorial_cache_max + 1; i <= limit; ++i)
        new_cache[i] = new_cache[i - 1] * (seq_t)i;

    /* replace old cache */
    free(factorial_cache);
    factorial_cache = new_cache;
    factorial_cache_max = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)           /* offset is 1; a(0) is undefined – return 0 */
        return 0;

    /* factorials up to n are required for all d|n */
    ensure_factorial_cache(n);

    seq_t total = 0;
    idx_t i;
    for (i = 1; i * i <= n; ++i) {
        if (n % i != 0)
            continue;

        /* divisor d = i */
        idx_t d1 = i;
        idx_t nd1 = n / d1;           /* n/d1 */
        seq_t term1 = (seq_t)phi_uint64(nd1);    /* φ(n/d1) */
        term1 *= pow_seq((seq_t)nd1, d1);         /* (n/d1)^d1 */
        term1 *= factorial_cache[d1];            /* d1! */
        total += term1;

        /* second divisor d = n/i, if distinct */
        idx_t d2 = n / i;
        if (d2 != d1) {
            idx_t nd2 = n / d2;           /* n/d2 */
            seq_t term2 = (seq_t)phi_uint64(nd2);
            term2 *= pow_seq((seq_t)nd2, d2);
            term2 *= factorial_cache[d2];
            total += term2;
        }
    }
    return total;
}
