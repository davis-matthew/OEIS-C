/*
 * A089611  Number of primes between n^2 and (n+1/3)^2.
 * https://oeis.org/A089611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = π( floor((n+1/3)^2) ) – π(n^2) (number of primes p with
 *            n^2 < p ≤ floor((n+1/3)^2))
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Build a new sieve up to 'limit' (inclusive) and store primes in prime_list.
   Existing prime_list is freed. */
static void sieve_upto(idx_t limit, idx_t **prime_list, size_t *prime_cnt)
{
    /* Allocate composite marker array */
    uint8_t *is_comp = (uint8_t *)calloc((size_t)(limit + 1), sizeof(uint8_t));
    if (!is_comp) return;            /* allocation failure ignored */

    size_t cap = limit / 10 + 10;    /* rough estimate */
    idx_t *pr = (idx_t *)malloc(cap * sizeof(idx_t));
    if (!pr) { free(is_comp); return; }

    size_t sz = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[(size_t)i]) {
            if (sz == cap) {
                cap *= 2;
                idx_t *tmp = (idx_t *)realloc(pr, cap * sizeof(idx_t));
                if (!tmp) break;   /* abort on allocation failure */
                pr = tmp;
            }
            pr[sz++] = i;
            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_comp[(size_t)j] = 1;
            }
        }
    }
    free(is_comp);
    *prime_list = pr;
    *prime_cnt   = sz;
}

/* binary search: returns the first index i with prime_list[i] > val.
   If all primes are ≤ val, returns prime_cnt. */
static size_t upper_bound(const idx_t *prime_list, size_t prime_cnt, idx_t val)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (prime_list[mid] <= val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* static cache of primes */
static idx_t *prime_list = NULL;
static size_t prime_cnt  = 0;
static idx_t sieve_limit = 0;

/* ----------------- main sequence function ----------------- */
seq_t a(idx_t n)
{
    /* compute lower = n^2, upper = n^2 + floor((2*n)/3) */
    unsigned __int128 nn = (unsigned __int128)n * n;
    idx_t lower  = (idx_t)nn;
    idx_t upper  = (idx_t)(nn + (unsigned __int128)(2 * n) / 3);

    if (upper > sieve_limit) {
        /* rebuild sieve up to new upper bound */
        if (prime_list) { free(prime_list); prime_list = NULL; }
        sieve_upto(upper, &prime_list, &prime_cnt);
        sieve_limit = upper;
    }

    /* Count primes ≤ upper and ≤ lower */
    size_t cnt_le_upper = upper_bound(prime_list, prime_cnt, upper);
    size_t cnt_le_lower = upper_bound(prime_list, prime_cnt, lower);

    return (seq_t)(cnt_le_upper - cnt_le_lower);
}
