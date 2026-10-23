/*
 * A106892  Primes of the form 3x^2+2xy+4y^2, with x and y nonnegative.
 * https://oeis.org/A106892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 884, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A106892
   Formula: primes of the form 3*x^2 + 2*x*y + 4*y^2, with non‑negative integers x,y.
   This implementation enumerates all values of the quadratic form up to an
   expanding bound, tests each value for primality with the supplied
   is_prime (static inline) routine, keeps the primes in a sorted array,
   and returns the n-th (1‑based) prime. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(uint64_t) */

static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb);
}

/* return the n-th prime of the form 3*x^2 + 2*x*y + 4*y^2   (n>=1) */
seq_t a(idx_t n)
{
    static uint64_t *buf = NULL;      /* buffer of found primes (unique, sorted) */
    static size_t buf_cnt = 0;        /* number of primes stored                */
    static size_t buf_cap = 0;        /* current capacity of the buffer         */
    static uint64_t last_limit = 0;   /* largest value already examined         */
    static uint64_t current_limit = 100; /* current upper bound for enumeration */

    /* build primes until we have at least n of them */
    while (buf_cnt < (size_t)n)
    {
        /* enlarge search interval */
        if (current_limit < 1000)
            current_limit = 1000;
        else
            current_limit *= 2;

        /* enumerate all candidate values in (last_limit, current_limit] */
        for (uint64_t y = 0; ; ++y)
        {
            unsigned __int128 y4 = (unsigned __int128)4 * y * y;
            if (y4 > current_limit) break;
            for (uint64_t x = 0; ; ++x)
            {
                unsigned __int128 val128 =
                    (unsigned __int128)3 * x * x
                  + (unsigned __int128)2 * x * y
                  + y4;
                if (val128 > current_limit) break;
                uint64_t val = (uint64_t)val128;
                if (val <= last_limit) continue;   /* already processed */
                if (val < 3) continue;             /* skip 0,1,2 */
                if (is_prime(val))
                {
                    if (buf_cnt == buf_cap)
                    {
                        buf_cap = buf_cap ? buf_cap * 2 : 64;
                        uint64_t *tmp = (uint64_t *)realloc(buf,
                                                             buf_cap * sizeof(uint64_t));
                        if (!tmp) abort();   /* allocation failure */
                        buf = tmp;
                    }
                    buf[buf_cnt++] = val;
                }
            }
        }
        last_limit = current_limit;

        /* sort and deduplicate the prime buffer */
        qsort(buf, buf_cnt, sizeof(uint64_t), cmp_uint64);
        size_t uniq = 0;
        for (size_t i = 0; i < buf_cnt; ++i)
        {
            if (i == 0 || buf[i] != buf[i - 1])
                buf[uniq++] = buf[i];
        }
        buf_cnt = uniq;
    }

    return (seq_t)buf[n - 1];
}
