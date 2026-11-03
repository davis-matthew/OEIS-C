/*
 * A205966  a(n) = Fibonacci(n)*A004016(n) for n>=1, with a(0)=1, where
 *          A004016(n) is the number of integer solutions (x,y) to x^2 + x*y +
 *          y^2 = n.
 * https://oeis.org/A205966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n) * A004016(n) (for n ≥ 1) a(0) = 1 where
 *            A004016(n) is the number of integer solutions (x,y) to x^2 + x*y
 *            + y^2 = n. The function implements the multiplicative
 *            representation count described in OEIS A004016
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 42, u64 n <= 90, u128 n <= 180,
 *            big any n
 * Validated: standard, pass, 68 terms through n = 973 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

/* multiplicative factor for prime powers in A004016 */
static seq_t
A004016(idx_t n)
{
    if (n == 0) return (seq_t)1;        /* not used but harmless */
    seq_t res = 1;
    /* skip all powers of 3 – they do not affect the sum */
    while (n % 3 == 0) n /= 3;

    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            idx_t exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            if (p % 3 == 1) {
                /* contributes (exp+1) to the product */
                res *= (seq_t)(exp + 1);
            } else /* p % 3 == 2 */ {
                /* must have even exponent, otherwise it contributes 0 */
                if (exp & 1) return (seq_t)0;
                /* even exponent → factor 1, nothing to do */
            }
        }
    }
    /* remaining prime factor, if any */
    if (n > 1) {
        if (n % 3 == 1) {
            res *= (seq_t)2;
        } else /* n % 3 == 2, exponent 1 (odd) */ {
            return (seq_t)0;
        }
    }
    return res * 6;                     /* final multiplier */
}

/* cache for computed terms; static to preserve across calls */
static seq_t *a_cache   = NULL;
static char  *a_comp    = NULL;        /* 0 = not cached, 1 = cached */
static idx_t  a_size    = 0;            /* current cache capacity */

seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)1;        /* explicit base case */

    /* allocate or grow cache to accommodate index n */
    if (n >= a_size) {
        idx_t new_size = (n + 1) * 2;
        seq_t *new_cache = mem_resize(seq_t, a_cache, new_size);
        char   *new_comp  = (char   *)realloc(a_comp,  new_size * sizeof(char));
        if (!new_cache || !new_comp) {   /* allocation failed – return 0 */
            return (seq_t)0;
        }
        /* initialize new slots */
        for (idx_t i = a_size; i < new_size; ++i) {
            new_cache[i] = 0;
            new_comp[i]  = 0;
        }
        a_cache = new_cache;
        a_comp  = new_comp;
        a_size  = new_size;
    }

    if (!a_comp[n]) {                    /* compute only once */
        seq_t valA = A004016(n);         /* A004016(n) */
        seq_t fibN = fib(n);             /* Fibonacci(n) */
        a_cache[n] = fibN * valA;        /* product in appropriate type */
        a_comp[n]  = 1;
    }

    return a_cache[n];
}
