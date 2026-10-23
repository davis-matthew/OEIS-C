/*
 * A024535  [ (4th elementary symmetric function of P(n))/(2nd elementary
 *          symmetric function of P(n)) ], where P(n) = {1, p(1), p(2), ...,
 *          p(n-1)}, where p(0) = 1.
 * https://oeis.org/A024535
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = e4(P(n)) / e2(P(n)), where * P(n) = {1, p(1), p(2), …,
 *            p(n‑1)} and p(k) is the k‑th prime
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 20, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

/* ------------------------------------------------------------ */
/* Global static data                                           */
/* ------------------------------------------------------------ */
static uint64_t *primes = NULL;          /* primes[0] = 2, primes[1] = 3, …          */
static size_t   primes_cnt = 0;          /* number of primes stored                */
static size_t   prime_limit = 0;         /* current sieve limit                     */

static size_t   arr_cap = 0;             /* capacity of the e2/e4 arrays           */
static seq_t  *e2_arr = NULL;           /* e2 values for indices 0 … n            */
static seq_t  *e4_arr = NULL;           /* e4 values for indices 0 … n            */

static idx_t    max_computed = 0;        /* largest n already processed            */

static seq_t e1_local = 0;              /* running sums of elementary symmetric  */
static seq_t e2_local = 0;
static seq_t e3_local = 0;
static seq_t e4_local = 0;

/* ------------------------------------------------------------ */
/* Ensure that at least `need` primes are available.           */
/* If necessary, increase the sieve limit until enough primes. */
/* ------------------------------------------------------------ */
static void
ensure_primes_count(idx_t need)
{
    if (primes_cnt >= (size_t)need)
        return;

    idx_t limit = (need * 20ULL > 200ULL) ? (need * 20ULL) : 200ULL;
    while (1) {
        if (primes)
            free(primes);
        build_primes_upto(limit, &primes, &primes_cnt);
        if (primes_cnt >= (size_t)need) {
            prime_limit = (size_t)limit;
            break;
        }
        limit *= 2;
    }
}

/* ------------------------------------------------------------ */
/* Ensure allocated arrays are large enough for index n.        */
/* ------------------------------------------------------------ */
static void
ensure_arr_capacity(idx_t n)
{
    if (arr_cap > (size_t)n)
        return;

    size_t new_cap = arr_cap ? arr_cap * 2 : 8;
    while (new_cap <= (size_t)n)
        new_cap *= 2;

    e2_arr = (seq_t *)realloc(e2_arr, (new_cap + 1) * sizeof(seq_t));
    e4_arr = (seq_t *)realloc(e4_arr, (new_cap + 1) * sizeof(seq_t));
    arr_cap = new_cap;
}

/* ------------------------------------------------------------ */
/* Main sequence function                                       */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)0;                 /* behavior below offset is irrelevant   */

    /* Extend precomputation up to n if necessary */
    if (max_computed < n) {
        ensure_arr_capacity(n);
        for (idx_t k = max_computed + 1; k <= n; ++k) {
            seq_t x;                     /* element to add to P(k)               */
            if (k == 1) {
                x = (seq_t)1;
            } else {
                /* need prime p(k‑1) */
                ensure_primes_count(k - 1);
                x = (seq_t)primes[k - 2];   /* primes[0] = 2                       */
            }

            seq_t old_e1 = e1_local;
            seq_t old_e2 = e2_local;
            seq_t old_e3 = e3_local;

            e4_local += x * old_e3;
            e3_local += x * old_e2;
            e2_local += x * old_e1;
            e1_local += x;

            e2_arr[k] = e2_local;
            e4_arr[k] = e4_local;
        }
        max_computed = n;
    }

    /* Integer division yields the required term */
    return e4_arr[n] / e2_arr[n];
}
