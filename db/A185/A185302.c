/*
 * A185302  a(n) = Sum_{d|n} d*sigma(n/d)^d.
 * https://oeis.org/A185302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d * sigma(n/d)^d where sigma(m) = Sum_{k|m} k
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 35, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation uses a simple sieve to pre‑compute σ values up
    to the requested n, and a fast binary exponentiation routine.
    It works for any integer type seq_t and for idx_t = uint64_t. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Fast exponentiation for seq_t base:  base^exp  (exp is idx_t) */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t power = base;
    while (exp != 0)
    {
        if (exp & (idx_t)1)
            result *= power;
        power *= power;
        exp >>= 1;
    }
    return result;
}

/* Cache of σ values: sigma_cache[i] = σ(i) for i <= sigma_cap */
static seq_t *sigma_cache = NULL;
static idx_t sigma_cap    = 0;

/* Recompute σ values for all numbers up to n (inclusive).  If the
   cache already covers the range, the call is a no‑op. */
static void compute_sigma_up_to(idx_t n)
{
    if (n <= sigma_cap)
        return;

    /* Allocate (or reallocate) the cache to size n+1 */
    seq_t *new_cache = (seq_t *)realloc(sigma_cache, (size_t)(n + 1) * sizeof(seq_t));
    if (!new_cache)
        exit(1);                  /* out‑of‑memory; termination is fine */
    sigma_cache = new_cache;

    /* Initialise the array to zero */
    for (idx_t i = 0; i <= n; ++i)
        sigma_cache[i] = (seq_t)0;

    /* Classic sieve for σ : add each divisor i to all multiples j = i·k */
    for (idx_t i = 1; i <= n; ++i)
    {
        for (idx_t j = i; j <= n; j += i)
            sigma_cache[j] += (seq_t)i;
    }

    sigma_cap = n;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n = 0, but return 0 */

    compute_sigma_up_to(n);        /* ensure σ is available for all divisors */

    seq_t sum = (seq_t)0;

    for (idx_t d = 1; d <= n; ++d)
    {
        if (n % d == 0)            /* d is a divisor of n */
        {
            idx_t q = n / d;                   /* n/d   */
            seq_t sigma_q = sigma_cache[q];    /* σ(n/d)  */
            seq_t pow_val = pow_seq(sigma_q, d);/* σ(n/d)^d */
            sum += pow_val * (seq_t)d;           /* d * σ(n/d)^d */
        }
    }

    return sum;
}
