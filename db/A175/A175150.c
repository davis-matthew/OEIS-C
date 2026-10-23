/*
 * A175150  a(1)=0. If d(n)>d(n-1), then a(n)=a(n-1)+1. If d(n)<d(n-1), then
 *          a(n)=a(n-1)-1. If d(n)=d(n-1), then a(n)=a(n-1). (d(n) is the
 *          number of divisors of n.)
 * https://oeis.org/A175150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A175150.
   a(1) = 0; for n ≥ 2
       a(n) = a(n‑1)+1 if d(n) > d(n‑1),
       a(n) = a(n‑1)‑1 if d(n) < d(n‑1),
       a(n) = a(n‑1) otherwise.
   d(n) is the number of divisors of n.  The implementation
   builds divisor counts on demand and stores the running
   a‑value in a static array.  It works for any integer
   types seq_t and idx_t supplied by the build. */

#include <stdint.h>
#include <stdlib.h>

/* static cache of largest computed index */
static idx_t max_n = 0;      /* largest n with valid data          */
static uint32_t *d = NULL;  /* divisor counts for 1 … max_n      */
static seq_t *a_arr = NULL; /* a(n) values for 1 … max_n         */

/* compute divisor count of n using trial division (fast enough for n<=10 000) */
static uint32_t divisor_count(uint64_t n)
{
    uint32_t result = 1;
    uint64_t k = n;
    for (uint64_t p = 2; p * p <= k; ++p) {
        if (k % p == 0) {
            uint32_t exp = 0;
            while (k % p == 0) {
                k /= p;
                ++exp;
            }
            result *= (exp + 1);
        }
    }
    if (k > 1) /* remaining prime factor */
        result *= 2;
    return result;
}

/* expand the static arrays to cover at least N (N>=1) */
static void ensure_upto(idx_t N)
{
    if (N <= max_n)                /* nothing to do        */
        return;

    /* enlarge divisor array */
    size_t new_sz = (size_t)N;
    uint32_t *new_d  = (uint32_t*)realloc(d, new_sz * sizeof(uint32_t));
    seq_t   *new_a  = (seq_t  *)realloc(a_arr, new_sz * sizeof(seq_t));
    if (!new_d || !new_a) abort();

    d    = new_d;
    a_arr = new_a;

    /* if this is the very first call, initialise d[0] and a[1] */
    if (max_n == 0) {
        d[0] = 1;            /* d(1) = 1 divisor                 */
        a_arr[0] = 0;        /* a(1) = 0                          */
        max_n = 1;
    }

    /* compute divisor counts for n = max_n+1 … N */
    for (size_t i = max_n + 1; i <= new_sz; ++i)
        d[i - 1] = divisor_count((uint64_t)i);

    /* compute a(n) values for the new indices */
    seq_t   current = a_arr[max_n - 1];   /* last known a value */
    for (size_t i = max_n + 1; i <= new_sz; ++i) {
        if (d[i - 1] > d[i - 2])  ++current;
        else if (d[i - 1] < d[i - 2]) --current;
        a_arr[i - 1] = current;
    }

    max_n = N;    /* update module‐wide maximum */
}

/* public function required by the harness */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined but harmless    */
        return 0;
    ensure_upto(n);      /* guarantee data availability */
    return a_arr[(size_t)n - 1];
}
