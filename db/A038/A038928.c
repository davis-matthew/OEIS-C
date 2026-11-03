/*
 * A038928  Primes p such that 47 is not a square mod p.
 * https://oeis.org/A038928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 885 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A038928: primes p such that 47 is not a quadratic residue modulo p. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

seq_t a(idx_t n)
{
    /* Returns the nth term of the sequence (1‑based indexing). */
    if (n == 0) return 0;          /* undefined offset – safety value */

    static seq_t *cache      = NULL; /* array of collected terms            */
    static size_t cache_cap  = 0;    /* current capacity of cache          */
    static size_t cache_cnt  = 0;    /* number of terms stored already     */
    static idx_t  candidate  = 3;    /* next odd candidate (start at 3)    */

    while (cache_cnt < (size_t)n)
    {
        if (is_prime(candidate))
        {
            int leg = jacobi_symbol(47ULL, candidate);
            if (leg == -1)            /* 47 is a non‑residue modulo candidate */
            {
                if (cache_cnt >= cache_cap)
                {
                    size_t newcap = (cache_cap == 0) ? 4 : cache_cap * 2;
                    seq_t *tmp = mem_resize(seq_t, cache, newcap);
                    if (!tmp) abort();
                    cache      = tmp;
                    cache_cap  = newcap;
                }
                cache[cache_cnt++] = (seq_t)candidate;
            }
        }
        candidate += 2;              /* skip even numbers */
    }

    return cache[n - 1];
}
