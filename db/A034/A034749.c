/*
 * A034749  Dirichlet convolution of Fibonacci numbers with Catalan numbers.
 * https://oeis.org/A034749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} F_d * C_{n/d-1}
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *fib_cache = NULL;
static size_t fib_cap   = 0;
static seq_t *cat_cache = NULL;
static size_t cat_cap   = 0;

static void ensure_fib(idx_t n)
{
    if ((size_t)n + 1 <= fib_cap) return;
    size_t new_cap = (size_t)n + 1;
    seq_t *p = (seq_t *)realloc(fib_cache, new_cap * sizeof(seq_t));
    if (!p) exit(1);
    fib_cache = p;
    for (idx_t i = fib_cap; (size_t)i <= n; ++i)
    {
        if (i == 0) fib_cache[i] = 0;
        else if (i == 1) fib_cache[i] = 1;
        else fib_cache[i] = fib_cache[i - 1] + fib_cache[i - 2];
    }
    fib_cap = new_cap;
}

static void ensure_cat(idx_t n)
{
    size_t needed = (n == 0) ? 1 : (size_t)n;   /* indices 0 .. n-1 */
    if (needed <= cat_cap) return;
    size_t new_cap = needed;
    seq_t *p = (seq_t *)realloc(cat_cache, new_cap * sizeof(seq_t));
    if (!p) exit(1);
    cat_cache = p;
    for (idx_t i = cat_cap; i < new_cap; ++i)
    {
        if (i == 0) cat_cache[i] = 1;
        else
        {
            seq_t prev = cat_cache[i - 1];
            seq_t num  = prev * (seq_t)(4 * i - 2);
            seq_t den  = (seq_t)(i + 1);
            cat_cache[i] = num / den;
        }
    }
    cat_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    ensure_fib(n);
    ensure_cat(n);               /* need indices up to n-1 */
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            idx_t d1 = i;
            idx_t d2 = n / i;

            idx_t m1 = d2;                     /* n/d1 */
            seq_t term1 = fib_cache[d1] * cat_cache[m1 - 1];
            sum += term1;

            if (d1 != d2)
            {
                idx_t m2 = d1;                 /* n/d2 */
                seq_t term2 = fib_cache[d2] * cat_cache[m2 - 1];
                sum += term2;
            }
        }
    }
    return sum;
}
