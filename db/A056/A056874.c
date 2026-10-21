/*
 * A056874  Primes of form x^2+xy+3y^2, discriminant -11.
 * https://oeis.org/A056874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 3254, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the list of primes that can be written as
 *   x^2 + x y + 3 y^2  (discriminant –11).
 * The n-th element (n≥1) is the n‑th prime p with Legendre symbol
 *   (-11 | p) = 1, or p = 11. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/jacobi_symbol.h"
#include "../utils/build_primes_upto.h"

/* return true iff prime p can be represented by x^2 + x y + 3 y^2   */
static bool representable(uint64_t p)
{
    if (p == 11)               /* 11 itself is representable   */
        return true;
    if ((p & 1) == 0)          /* even primes are not representable */
        return false;

    /* Legendre symbol (-11|p) = (-1|p) * (11|p)                    */
    int leg  = (p & 3UL) == 1 ? 1 : -1;   /* (-1|p)  */
    int jac  = jacobi_symbol(11, p);      /* (11|p)  */
    return jac == leg;                    /* (-11|p)=1 */
}

/* estimate an upper bound for the n‑th representable prime  */
static idx_t estimate_limit(idx_t n)
{
    /* about twice as many ordinary primes are needed; use a safe factor   */
    return n * 20 + 100;
}

/* static cache of all primes up to the current upper limit             */
static uint64_t *primes_cache = NULL;
static size_t   primes_cnt   = 0;
static idx_t    primes_limit = 0;

/* ensure that primes_cache contains all primes ≤ limit                */
static void ensure_primes(idx_t limit)
{
    if (limit <= primes_limit)
        return;                                 /* already sufficient */

    uint64_t *new_primes = NULL;
    size_t    new_cnt    = 0;
    build_primes_upto(limit, &new_primes, &new_cnt);  /* unsigned long long */

    /* free old cache */
    if (primes_cache != NULL)
        free(primes_cache);

    primes_cache = new_primes;
    primes_cnt   = new_cnt;
    primes_limit = limit;
}

/* external sequence function */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n=0 – return 0 safely     */
        return 0;

    idx_t limit = estimate_limit(n);

    for (;;)
    {
        ensure_primes(limit);

        idx_t count = 0;
        for (size_t i = 0; i < primes_cnt; ++i)
        {
            uint64_t p = primes_cache[i];
            if (representable(p))
            {
                ++count;
                if (count == n)
                    return (seq_t)p;
            }
        }

        /* not enough representable primes found – enlarge */
        limit *= 2;
    }
}
