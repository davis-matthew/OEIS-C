/*
 * A121325  Irregular triangle read by rows in which the n-th row gives the
 *          prime factors of 4*n^2 + 1 with repetition.
 * https://oeis.org/A121325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A121325 – Irregular triangle read by rows.
 *  The n‑th term is the n‑th prime (with multiplicity) of 4*r^2+1
 *  where r is the row number.  Rows are read sequentially; each row
 *  lists the prime factors of 4*r^2+1 in non‑decreasing order.
 *
 *  Function: seq_t a(idx_t n)
 *
 *  Implementation caveats:
 *  - Uses an on‑demand prime sieve limited to the needed range.
 *  - Computes the required row by accumulating factor counts.
 *  - Works for any order of calls; reuses the prime list.
 */

#include <stdlib.h>
#include <stdint.h>
/*  Prime maker (returns an array of primes up to 'limit')   */
#include "../utils/build_primes_upto.h"

/*--------------------------------------------------------------------*/
/*  Global data for the dynamic prime list.   */
static uint64_t *prime_list  = NULL;   /* array of primes   */
static size_t   prime_count = 0;       /* number of primes  */
static uint64_t prime_max   = 0;       /* max prime value   */

/*====================================================================*/
/*  Ensure all primes ≤ limit are present in prime_list.   */
static void ensure_primes(uint64_t limit)
{
    if (prime_list && prime_max >= limit)
        return;                       /* already sufficient */

    /* Build a new prime list up to 'limit'.   */
    uint64_t *new_primes = NULL;
    size_t    new_cnt   = 0;
    build_primes_upto((idx_t)limit, &new_primes, &new_cnt);

    free(prime_list);                 /* discard old list */
    prime_list  = new_primes;
    prime_count = new_cnt;
    prime_max   = limit;
}

/*====================================================================*/
/*  Count the number of prime factors (with multiplicity) of
 *  4*r^2+1.  All primes up to √(4*r^2+1) are already available.   */
static size_t row_factor_count(uint64_t r)
{
    uint64_t m     = 4ULL * r * r + 1ULL;
    uint64_t tmp   = m;
    size_t   cnt   = 0;

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > tmp)
            break;
        while (tmp % p == 0) {
            tmp /= p;
            ++cnt;
        }
    }
    if (tmp > 1)          /* a remaining prime factor */
        ++cnt;
    return cnt;
}

/*====================================================================*/
/*  Return the (k‑th, 0‑based) prime factor of 4*r^2+1.   */
static seq_t kth_row_factor(uint64_t r, size_t k)
{
    uint64_t m     = 4ULL * r * r + 1ULL;
    uint64_t tmp   = m;
    size_t   idx   = 0;

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > tmp)
            break;
        size_t pow = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++pow;
        }
        if (pow) {
            if (idx + pow > k)
                return (seq_t)p;
            idx += pow;
        }
    }
    if (tmp > 1) {          /* remaining prime factor          */
        if (idx + 1 > k)
            return (seq_t)tmp;
    }
    /* k is out of range (should not happen) */
    return 0;
}

/*====================================================================*/
/*  Main sequence function.    */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* offset starts at 1 */

    /* Ensure the prime table is large enough for the maximum row
     * that could be needed.  A row index can never exceed n
     * (each row contributes at least one factor).            */
    uint64_t limit = 2ULL * (uint64_t)n + 1ULL;   /* ≥ sqrt(4*r^2+1) */
    ensure_primes(limit);

    /* Find the row that contains the n‑th factor.  Cumulative
     * counting is O(n) and fast enough for n up to at least 10‑000. */
    uint64_t row   = 0;
    size_t   cum   = 0;     /* cumulative number of factors so far */

    for (uint64_t r = 1; ; ++r) {
        size_t cnt = row_factor_count(r);
        if (cum + cnt >= (size_t)n) {
            row = r;
            break;
        }
        cum += cnt;
    }

    /* Position within that row (0‑based). */
    size_t pos = (size_t)n - cum - 1;

    return kth_row_factor(row, pos);
}
