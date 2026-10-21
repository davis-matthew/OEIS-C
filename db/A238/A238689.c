/*
 * A238689  Table read by rows: first row is {1}; for n >1, T(n, k) is the
 *          k-th largest prime factor of n (repeated prime factors are counted
 *          repeatedly).
 * https://oeis.org/A238689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) gives the n-th element when the rows of the table of prime
 *            factors (reverse order) are concatenated. The table is 1; 2; 3;
 *            2,2; 2,5; 3; 2,7; … where row n lists the prime factors of n in
 *            descending order (multiplicities counted)
 * Range:     u8 n <= 674, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- integer square root for 64‑bit values ---------- */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {
            low = 1;
            continue;
        }
        if (mid > n / mid) {
            high = mid - 1;
        } else {
            idx_t sq = mid * mid;
            if (sq == n)
                return mid;
            if (sq < n)
                low = mid + 1;
            else
                high = mid - 1;
        }
    }
    return high;
}

/* ---------- prime list generation (sieve by trial division) ---------- */
static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;
static size_t prime_cap = 0;
static uint64_t prime_search_limit = 1;

static void ensure_primes_up_to(idx_t limit)
{
    if (prime_search_limit >= limit)
        return;

    if (prime_cap == 0) {
        prime_cap = 64;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_list)
            exit(1);
    }

    for (uint64_t cand = prime_search_limit + 1; cand <= limit; ++cand) {
        bool is_prime = true;
        uint64_t sqround = isqrt_u64(cand);
        for (size_t i = 0; i < prime_cnt &&
             prime_list[i] <= sqround; ++i) {
            if (cand % prime_list[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_cnt >= prime_cap) {
                prime_cap *= 2;
                uint64_t *tmp =
                    (uint64_t *)realloc(prime_list,
                                        prime_cap * sizeof(uint64_t));
                if (!tmp)
                    exit(1);
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = cand;
        }
    }
    prime_search_limit = limit;
}

/* ---------- factorization into descending prime factors ----------
   Out array must hold at least 64 elements.  Out count is returned
   via out_cnt. */
static void factor_desc(idx_t n,
                        seq_t *out, size_t *out_cnt)
{
    if (n == 1) {
        *out_cnt = 0;
        return;
    }

    seq_t asc_factors[64];
    size_t asc_cnt = 0;
    idx_t rem = n;
    idx_t sqrt_limit = isqrt_u64(rem);
    ensure_primes_up_to(sqrt_limit);

    for (size_t i = 0; i < prime_cnt &&
         prime_list[i] <= sqrt_limit; ++i) {
        uint64_t p = prime_list[i];
        while (rem % p == 0) {
            if (asc_cnt < 64)
                asc_factors[asc_cnt++] = (seq_t)p;
            rem /= p;
        }
        if (rem == 1)
            break;
        sqrt_limit = isqrt_u64(rem);
    }

    if (rem > 1 && asc_cnt < 64)
        asc_factors[asc_cnt++] = (seq_t)rem;

    /* reverse to descending order */
    *out_cnt = asc_cnt;
    for (size_t i = 0; i < asc_cnt; ++i)
        out[i] = asc_factors[asc_cnt - 1 - i];
}

/* ---------- main function computing a(n) ---------- */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;               /* first term */

    idx_t remaining = n - 1;           /* skip the initial 1 */
    idx_t cur = 2;                     /* start from row 2 */

    while (true) {
        seq_t fac[64];
        size_t fac_cnt = 0;
        factor_desc(cur, fac, &fac_cnt);

        if (remaining <= fac_cnt) {
            /* remaining is 1‑based index within this row */
            return fac[remaining - 1];
        }
        remaining -= fac_cnt;
        ++cur;
    }
}
