/*
 * A023179  Numbers k such that Fib(k) == 21 (mod k).
 * https://oeis.org/A023179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers k such that Fib(k) == 21 (mod k)
 * Range:     u8 n <= 6, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute Fibonacci F(n) modulo n efficiently. */
static seq_t fib_mod(idx_t n)
{
    if (n == 0) return 0;
    seq_t a = 0;               /* F(0)   */
    seq_t b = 1;               /* F(1)   */
    seq_t mod = (seq_t)n;      /* modulo n */

    /* Find the position of the most significant bit of n */
    int msb = 63 - __builtin_clzll(n);
    for (int i = msb; i >= 0; --i)
    {
        /* t = (2*b - a) mod mod, computed safely */
        seq_t t = (b + b) % mod;          /* t = 2*b mod mod */
        if (t >= a)
            t -= a;
        else
            t = t + mod - a;              /* ensure t modulo mod */

        seq_t c = (a * t) % mod;          /* F(2k)        */
        seq_t d = ((a * a) % mod + (b * b) % mod) % mod;  /* F(2k+1)    */

        if ((n >> i) & 1)
        {
            a = d;
            b = (c + d) % mod;
        }
        else
        {
            a = c;
            b = d;
        }
    }
    return a % mod;        /* return F(n) mod n */
}

/* Return the n-th integer k satisfying Fib(k) == 21 (mod k). */
seq_t a(idx_t n)
{
    /* Static storage for memoization */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t last_k = 0;

    if (n <= cache_size)
        return cache[n - 1];

    /* Extend cache until we have enough terms */
    for (;;)
    {
        ++last_k;
        seq_t f = fib_mod(last_k);
        seq_t r = (21 % (seq_t)last_k);
        if (f == r)
        {
            seq_t *tmp = (seq_t *)realloc(cache, (cache_size + 1) * sizeof(seq_t));
            if (!tmp)
                abort(); /* out of memory */
            cache = tmp;
            cache[cache_size] = last_k;
            ++cache_size;
            if (cache_size >= n)
                return cache[n - 1];
        }
    }
}
