/*
 * A181754  a(1) = 1, a(2) = 2. For n >= 3, a(n) is found by concatenating the
 *          first n-1 terms of the sequence and then dividing the resulting
 *          number by a(n-1).
 * https://oeis.org/A181754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(1)=1, a(2)=2, a(3)=6. For n>=2 : a(n+2) = 10^F(n) * a(n) + 1,
 *            where F(n) is the nth Fibonacci number
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 10,
 *            big any n
 * Helpers:   pow10, seq_from_str, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Standard headers – needed for both C11 and C++17.  */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*  Utility headers that provide the required big‑integer operations.  */
#include "../utils/pow10.h"          /* seq_t pow10(idx_t e); */
#include "../utils/seq_from_str.h"   /* seq_t seq_from_str(const char *s); */
#include "../utils/seq_mul.h"        /* seq_t seq_mul(seq_t a, seq_t b); */

/*  Cache for previously computed terms.  */
static seq_t *cache = NULL;          /* elements 0..n  */
static size_t cache_len = 0;         /* number of allocated slots */

/*  Fibonacci numbers  – stored as 64‑bit unsigned integers.  */
static idx_t *fib = NULL;            /* fib[0] = 0, fib[1] = 1, etc.  */
static size_t fib_len = 0;           /* number of allocated slots */

/*  Ensure that fib[0..up_to] exist, creating any missing entries.  */
static void ensure_fib(size_t up_to)
{
    if (fib_len > up_to)
        return;

    size_t new_len = up_to + 1;                      /* need space for index up_to  */
    idx_t *tmp = (idx_t *)realloc(fib, new_len * sizeof(idx_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    fib = tmp;

    if (fib_len == 0) {                             /* first call  */
        fib[0] = (idx_t)0;
        fib[1] = (idx_t)1;
        fib_len = 2;
    }

    for (size_t i = fib_len; i <= up_to; ++i)
        fib[i] = fib[i - 1] + fib[i - 2];

    fib_len = new_len;
}

/*  Ensure that cache[1..n] exist, computing any missing terms.  */
static void ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;                                      /* already large enough */

    size_t new_len = (size_t)n + 1;                  /* include index 0 */
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    /* Zero initialise the newly allocated part (only works if seq_t is trivially
       constructible – see assumptions in the problem statement).  */
    if (new_len > cache_len)
        memset(tmp + cache_len, 0, (new_len - cache_len) * sizeof(seq_t));
    cache = tmp;
    cache_len = new_len;

    /*  Initialise base values if they are still zero.  */
    if (n >= 1 && cache[1] == (seq_t)0)
        cache[1] = seq_from_str("1");
    if (n >= 2 && cache[2] == (seq_t)0)
        cache[2] = seq_from_str("2");
    if (n >= 3 && cache[3] == (seq_t)0)
        cache[3] = seq_from_str("6");

    /*  Compute remaining terms.  */
    for (idx_t i = 4; i <= n; ++i) {
        if (cache[i] != (seq_t)0)
            continue;          /* already computed by a previous call */

        /*  Recurrence: a(i) = 10^F(i-2) * a(i-2) + 1  for i >= 4  */
        ensure_fib((size_t)(i - 2));            /* make sure fib[i-2] exists */
        seq_t factor = pow10(fib[i - 2]);       /* 10^F(i-2) */
        seq_t part  = seq_mul(factor, cache[i - 2]); /* factor * a(i-2) */
        cache[i] = part + seq_from_str("1");    /* + 1 */
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;            /* undefined for n<1 – return 0 */
    ensure_cache(n);
    return cache[n];
}
