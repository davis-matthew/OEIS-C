/*
 * A134412  a(n) = the numerator of (Sum_{k=1..n} 1/A134411(k)).
 * https://oeis.org/A134412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   gcd, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A134412.
   a(n) = numerator of the sum_{k=1..n} 1/A134411(k).
   A134411(k) is defined as the smallest positive integer d such that
   when 1/d is added to the current sum, the resulting fraction has a
   prime numerator.  The sequence is computed iteratively, maintaining
   the current reduced fraction and searching for the smallest such d
   at each step.  This implementation uses only the operations allowed
   on seq_t, with helper functions for gcd and primality testing.  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/is_prime_big.h"

static seq_t *num = NULL;          /* numerators of the partial sums */
static seq_t *den = NULL;          /* denominators of the partial sums */
static idx_t   max_computed = 0;   /* largest n whose values are known */
static size_t  capacity    = 0;    /* allocated array size */

/* Ensure that the arrays have space for at least n+1 entries
   (indexing starts at 1). */
static void ensure_capacity(idx_t n)
{
    if (capacity >= (size_t)(n + 1))
        return;
    size_t newcap = capacity ? capacity : 1;
    while (newcap <= (size_t)(n + 1))
        newcap <<= 1;
    seq_t *newnum = (seq_t *)realloc(num, newcap * sizeof(seq_t));
    seq_t *newden = (seq_t *)realloc(den, newcap * sizeof(seq_t));
    /* if malloc returned NULL, the program will terminate naturally */
    if (newcap > capacity) {
        /* initialize new cells to 0 */
        for (size_t i = capacity; i < newcap; ++i) {
            newnum[i] = 0;
            newden[i] = 0;
        }
    }
    num   = newnum;
    den   = newden;
    capacity = newcap;
}

/* Return the nth term of A134412. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* undefined for n = 0, but guard */

    ensure_capacity(n);

    /* initialise for the very first term */
    if (max_computed == 0) {
        num[1] = (seq_t)1;
        den[1] = (seq_t)1;
        max_computed = 1;
    }

    /* fast return if already computed */
    if (n <= max_computed)
        return num[n];

    /* current fraction before extending */
    seq_t p = num[max_computed];
    seq_t q = den[max_computed];

    for (idx_t k = max_computed + 1; k <= n; ++k) {
        idx_t d = 1;
        while (true) {
            /* N = p*d + q , D = q*d  (all on seq_t) */
            seq_t N = p * (seq_t)d + q;
            seq_t D = q * (seq_t)d;
            seq_t g = gcd(N, D);          /* reduce numerator and denom */
            seq_t pp = N / g;              /* reduced numerator            */
            if (is_prime_big(pp)) {        /* stop at smallest d with prime numerator */
                seq_t qq = D / g;          /* reduced denominator            */
                num[k] = pp;
                den[k] = qq;
                p = pp;                    /* set for next step              */
                q = qq;
                break;
            }
            ++d;                           /* try next candidate d           */
        }
    }

    max_computed = n;
    return num[n];
}
