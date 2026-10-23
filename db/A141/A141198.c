/*
 * A141198  a(n) is the number of divisors of n that are each one more than a
 *          power of a prime.
 * https://oeis.org/A141198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of divisors d of n such that d-1 is a power of
 *            a prime (including 1 as p^0)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *prime_tbl = NULL;
static size_t prime_cnt = 0;
static idx_t prime_limit = 0;

/* Integer square root: floor(sqrt(n)) */
static inline idx_t floor_sqrt(idx_t v)
{
    idx_t lo = 0, hi = v;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid == 0) {
            lo = 1;
            continue;
        }
        if (mid > v / mid) {            /* mid*mid > v */
            if (mid == 0) break;
            hi = mid - 1;
        } else {
            if (mid * mid == v) return mid;
            lo = mid + 1;
        }
    }
    return hi;
}

/* Generate primes up to 'limit' (inclusive) and store them in prime_tbl. */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit) return;
    size_t sz = (size_t)limit + 1;
    bool *is_comp = (bool*)malloc(sz);
    if (!is_comp) return;  /* never happens for given limits */
    memset(is_comp, 0, sz);
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            ++cnt;
            if (i * i <= (size_t)limit) {
                for (size_t m = i * i; m <= (size_t)limit; m += i) {
                    is_comp[m] = true;
                }
            }
        }
    }
    uint64_t *new_tbl = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!new_tbl) { free(is_comp); return; }  /* never happens */
    size_t pos = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) new_tbl[pos++] = (uint64_t)i;
    }
    free(is_comp);
    free(prime_tbl);
    prime_tbl = new_tbl;
    prime_cnt = cnt;
    prime_limit = limit;
}

/* Test whether x is a power of a prime (including 1). */
static bool is_prime_power(uint64_t x)
{
    if (x <= 1) return x == 1;   /* 1 is p^0, 0 is not tested here */
    uint64_t temp = x;
    uint64_t p = 0;
    for (size_t i = 0; i < prime_cnt && prime_tbl[i] <= temp / prime_tbl[i]; ++i) {
        if (temp % prime_tbl[i] == 0) {
            p = prime_tbl[i];
            while (temp % p == 0) temp /= p;
            break;
        }
    }
    if (p == 0) return true;  /* x itself is prime */
    return temp == 1;         /* only one distinct prime factor left */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    idx_t lim = floor_sqrt(n);
    ensure_primes_upto(lim);
    seq_t cnt = 0;
    for (idx_t i = 1; i <= lim; ++i) {
        if (n % i == 0) {
            /* divisor i */
            if (i > 1) {
                uint64_t d = i;
                if (is_prime_power(d - 1)) ++cnt;
            }
            /* paired divisor n/i */
            idx_t j = n / i;
            if (j != i && j > 1) {
                uint64_t d = j;
                if (is_prime_power(d - 1)) ++cnt;
            }
        }
    }
    return cnt;
}
