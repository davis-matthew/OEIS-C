/*
 * A179214  Product of squarefree numbers between n and 2*n (inclusive).
 * https://oeis.org/A179214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   build_primes_upto, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS: A179214
 *  Formula: a(n) = ∏_{k=n}^{2n} k^A008966(k)
 *  where A008966(k) is 1 iff k is squarefree, 0 otherwise.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"   /* build_primes_upto(idx_t, uint64_t **, size_t *) */
#include "../utils/int_sqrt.h"           /* int_sqrt(idx_t) */

/* ------------------------------------------------------------
 *  Helper: set of primes up to a given limit (inclusive).
 *  The array is rebuilt only when a larger limit is required.
 * ------------------------------------------------------------ */
static uint64_t *prime_list   = NULL;
static size_t   prime_count   = 0;
static idx_t    prime_max_lim = 0;

static void
ensure_primes(idx_t limit)
{
    if (limit <= prime_max_lim)
        return;

    free(prime_list);
    build_primes_upto(limit, &prime_list, &prime_count);
    prime_max_lim = limit;
}

/* ------------------------------------------------------------
 *  Test whether a number x (<= 2n) is squarefree.
 *  Uses trial division by the pre‑computed primes up to sqrt(x).
 * ------------------------------------------------------------ */
static bool
is_squarefree(idx_t x)
{
    if (x <= 1)
        return true;          /* 1 is considered squarefree */

    idx_t rem = x;

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if ((idx_t)p * (idx_t)p > rem)
            break;

        int exp = 0;
        while (rem % p == 0) {
            rem /= p;
            ++exp;
            if (exp >= 2)
                return false;   /* square factor found */
        }

        if (rem == 1)
            break;
    }

    /* Remaining part larger than sqrt(x) can have exponent at most 1 */
    return true;
}

/* ------------------------------------------------------------
 *  Sequence function a(n)
 * ------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    idx_t upper = 2 * n;

    /* Ensure we have primes up to sqrt(2n) for square‑free testing */
    idx_t prime_limit = (idx_t)int_sqrt(upper) + 1;
    ensure_primes(prime_limit);

    seq_t prod = (seq_t)1;
    for (idx_t k = n; k <= upper; ++k) {
        if (is_squarefree(k))
            prod = prod * (seq_t)k;
    }
    return prod;
}
