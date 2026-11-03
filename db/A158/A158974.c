/*
 * A158974  a(n) is the number of numbers k <= n such that not all proper
 *          divisors of k are divisors of n.
 * https://oeis.org/A158974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #k (1≤k≤n) for which some proper divisor d of k does not
 *            divide n
 * Range:     u8 n <= 325, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 92 terms through n = 9818 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/*------------------------------------------------------------------*/
static bool
has_non_dividing_proper_divisor(idx_t k, idx_t n, const idx_t *spf)
{
    if (k <= 1)
        return false;            /* 1 has no proper divisors */

    /* Factor k into primes and exponents */
    idx_t primes[20];
    int   exps[20];
    int   pcnt = 0;
    idx_t tmp = k;
    while (tmp > 1) {
        idx_t p = spf[tmp];
        int   e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        primes[pcnt] = p;
        exps[pcnt]   = e;
        ++pcnt;
    }

    /* Enumerate all divisors of k (including 1 and k) */
    idx_t divisors[128];
    int   dcnt = 1;
    divisors[0] = 1;
    for (int i = 0; i < pcnt; ++i) {
        idx_t p = primes[i];
        int   e = exps[i];
        int   prev = dcnt;
        idx_t mult = 1;
        for (int t = 1; t <= e; ++t) {
            mult *= p;
            for (int j = 0; j < prev; ++j) {
                divisors[dcnt++] = divisors[j] * mult;
            }
        }
    }

    /* Test each proper divisor */
    for (int i = 0; i < dcnt; ++i) {
        idx_t d = divisors[i];
        if (d == k)         /* skip k itself */
            continue;
        if (n % d != 0)     /* found a divisor that does not divide n */
            return true;
    }
    return false;
}

/*------------------------------------------------------------------*/
/*  Smallest prime factor sieve (up to a dynamic limit).            */
/*------------------------------------------------------------------*/
static idx_t cur_spf_limit = 0;
static idx_t *spf_arr      = NULL;

/* Allocate / enlarge the SPF table to include all numbers ≤ limit. */
static void
ensure_spf(idx_t limit)
{
    if (limit <= cur_spf_limit)
        return;

    free(spf_arr);
    spf_arr = (idx_t *)calloc((size_t)(limit + 1), sizeof(idx_t));
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf_arr[i] == 0) {
            spf_arr[i] = i;
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf_arr[j] == 0)
                        spf_arr[j] = i;
            }
        }
    }
    cur_spf_limit = limit;
}

/*------------------------------------------------------------------*/
/*  Check whether k has a proper divisor that is not a divisor of n.*/

/*------------------------------------------------------------------*/
/*  Public sequence function.                                       */
/*------------------------------------------------------------------*/
seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;

    ensure_spf(n);

    seq_t count = 0;
    for (idx_t k = 1; k <= n; ++k)
        if (has_non_dividing_proper_divisor(k, n, spf_arr))
            ++count;
    return count;
}
