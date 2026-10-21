/*
 * A056565  Fibonomial coefficients.
 * https://oeis.org/A056565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonomial(n+7, 7) = ∏_{i=1..7} F(n+8-i)/F(i) * where
 *            F(0)=0, F(1)=1, and a(0)=1, a(1)=21, a(2)=714, ... * * The
 *            implementation caches Fibonacci numbers and reduces the * common
 *            factor 3120 = 1*1*2*3*5*8*13 against the product of the * seven
 *            Fibonacci terms
 * Range:     u8 none, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>
#include "../utils/gcd.h"

/* Lazy Fibonacci cache shared by all calls */
static seq_t *fib_cache = NULL;
static idx_t fib_cache_len = 0;

/* Ensure fib_cache contains at least fib[0..n] */
static void ensure_fib(idx_t n)
{
    if (fib_cache_len == 0) {
        fib_cache_len = 2;                    /* indices 0 and 1 */
        fib_cache = (seq_t *)malloc(fib_cache_len * sizeof(seq_t));
        assert(fib_cache != NULL);
        fib_cache[0] = 0;
        fib_cache[1] = 1;
    }

    if (fib_cache_len > n)
        return;

    idx_t new_len = fib_cache_len;
    while (new_len <= n)
        new_len <<= 1;                        /* double until big enough */

    seq_t *tmp = (seq_t *)realloc(fib_cache, new_len * sizeof(seq_t));
    assert(tmp != NULL);
    fib_cache = tmp;

    /* Compute new Fibonacci values */
    for (idx_t i = fib_cache_len; i < new_len; ++i)
        fib_cache[i] = fib_cache[i - 1] + fib_cache[i - 2];

    fib_cache_len = new_len;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* We need Fibonacci numbers up to n+7 */
    ensure_fib(n + 7);

    seq_t denom = 3120;                      /* 1*1*2*3*5*8*13 */
    seq_t result = 1;

    /* Compute product with on-the-fly cancellation */
    for (idx_t j = n + 1; j <= n + 7; ++j) {
        seq_t f = fib_cache[j];

        /* Cancel common factors with remaining denominator */
        while (denom > 1) {
            seq_t g = gcd(f, denom);
            if (g == 1)
                break;
            f /= g;
            denom /= g;
        }

        result *= f;
    }

    /* Denominator should now be 1 */
    return result;
}
