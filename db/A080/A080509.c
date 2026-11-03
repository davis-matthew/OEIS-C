/*
 * A080509  Last term in n-th row of A080508.
 * https://oeis.org/A080509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (primorial(n-1))^n / (n-1)! (n≠2), a(2) = 4 (special
 *            case)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 13,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 54 (2026-11-01)
 * Helpers:   build_primes_upto, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses the product
   a(n) = ∏_{p≤n-1} p^{ n - v_p((n-1)!) } ,
   where v_p denotes the exponent of prime p in a factorial.
   The list of primes <= n-1 is cached across calls. */

#include <stdint.h>
#include <stdlib.h>

#include <stdbool.h>
#include <string.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static idx_t primes_max   = 0;          /* highest limit for which primes are known    */
static uint64_t *primes    = NULL;       /* array of primes up to primes_max            */
static size_t primes_cnt   = 0;          /* number of primes stored                      */

/* Ensure that primes[0..primes_cnt-1] contain all primes <= limit. */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= primes_max)
        return;                                      /* already available */

    /* allocate new list */
    uint64_t *new_primes = NULL;
    size_t new_cnt = 0;
    build_primes_upto(limit, &new_primes, &new_cnt);

    /* free previous list */
    free(primes);
    primes   = new_primes;
    primes_cnt = new_cnt;
    primes_max  = limit;
}

/* a(n) computation */
seq_t a(idx_t n)
{
    if (n < 1)                      /* behaviour below offset is irrelevant */
        return (seq_t)0;

    if (n == 1)
        return (seq_t)1;              /* primorial(0)^1 / 0! = 1 */

    if (n == 2)                      /* special case */
        return (seq_t)4;

    /* for n >= 3 */
    const idx_t k = n - 1;            /* upper bound for primes */

    /* build primes up to k */
    ensure_primes_upto(k);

    seq_t result = (seq_t)1;

    /* loop over each prime <= k */
    for (size_t i = 0; i < primes_cnt; ++i)
    {
        uint64_t p = primes[i];
        if (p > (uint64_t)k)           /* primes array may contain larger values when k larger */
            break;

        /* compute exponent e = n - v_p(k!)   */
        idx_t e  = n;
        idx_t tmp = k;
        while (tmp)
        {
            tmp /= p;
            e -= tmp;
        }
        /* e may be 0, in which case big_pow returns 1 */
        seq_t powv = ipow((seq_t)p, (seq_t)e);
        result = seq_mul(result, powv);
    }

    return result;
}
