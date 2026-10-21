/*
 * A076842  Numerators of sequence of fractions defined by a(1) = a(2) = 1;
 *          for n > 2, a(n) = (a(n-1)+a(n-2)+1)/a(n-2).
 * https://oeis.org/A076842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   Numerators of the fraction sequence a(1) = a(2) = 1 for n > 2,
 *            a(n) = (a(n-1)+a(n-2)+1)/a(n-2) (a(n) is a rational number) The
 *            returned value is the numerator of that fraction, reduced to
 *            lowest terms
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 13, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   alloc_seq_array, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"          /* seq_t seq_gcd(seq_t a, seq_t b) */
#include "../utils/alloc_seq_array.h"

#if defined(__cplusplus)
#include <type_traits>
/* big_t is used for intermediate arithmetic.  For integral seq_t we use
   __uint128_t, for arbitrary‑precision classes we use seq_t itself. */
using big_t = typename std::conditional<std::is_integral<seq_t>::value,
                                       __uint128_t, seq_t>::type;
#else
/* In plain C we assume seq_t is an unsigned integer type. */
typedef __uint128_t big_t;
#endif

/* -------- Memoization infrastructure ------------------------------------ */
static seq_t *num = NULL;         /* numerators, 1‑based */
static seq_t *den = NULL;         /* denominators, 1‑based */
static size_t cache_cap = 0;      /* array capacity (number of elements) */
static idx_t  computed   = 0;     /* largest index computed */

/* ensure_cache: make sure num[1..n] and den[1..n] exist       */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;          /* include index 0 which is unused */
    if (need <= cache_cap) return;

    size_t new_cap = need > 1 ? need * 2 : 2;

    seq_t *new_num = alloc_seq_array(new_cap);
    seq_t *new_den = alloc_seq_array(new_cap);

    if (num)
    {
        for (size_t i = 0; i < cache_cap; ++i)
        {
            new_num[i] = num[i];
            new_den[i] = den[i];
        }
    }

    /* no free; memory will be reclaimed at program exit */
    num = new_num;
    den = new_den;
    cache_cap = new_cap;
}

/* gcd_big: Euclidean algorithm for big_t (works for both __uint128_t
   and arbitrary‑precision seq_t because the operators +,-,*,/,% are
   provided for seq_t). */
static big_t gcd_big(big_t a, big_t b)
{
    while (b != 0)
    {
        big_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* -------- Main function ----------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;     /* undefined for n < 1 */

    if (!num)                       /* initialise cache */
        ensure_cache(2);

    /* compute terms up to n if not already done */
    if ((idx_t)computed < n)
    {
        ensure_cache(n);

        for (idx_t i = (idx_t)computed + 1; i <= n; ++i)
        {
            if (i <= 2)          /* base values */
            {
                num[i] = (seq_t)1;
                den[i] = (seq_t)1;
                continue;
            }

            /* p1/q1 = a(i-1),   p2/q2 = a(i-2) */
            seq_t p1 = num[i-1];
            seq_t q1 = den[i-1];
            seq_t p2 = num[i-2];
            seq_t q2 = den[i-2];

            /* Compute C = p1*q2 + p2*q1 + q1*q2   (all in big_t) */
            big_t C = (big_t)p1 * q2 + (big_t)p2 * q1 + (big_t)q1 * q2;

            /* Compute D = q1 * p2 */
            big_t D = (big_t)q1 * p2;

            /* Reduce fraction: divide by GCD */
            big_t G = gcd_big(C, D);

            num[i] = (seq_t)(C / G);
            den[i] = (seq_t)(D / G);
        }

        computed = n;
    }

    return num[n];
}
