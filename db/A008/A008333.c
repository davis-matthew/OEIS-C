/*
 * A008333  Sum of divisors of p+1, p prime.
 * https://oeis.org/A008333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of divisors of (p_n + 1), where p_n is the nth prime
 * Range:     u8 n <= 27, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* --- integer square root ------------------------------------------------- */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t x = 0, bit = ((idx_t)1) << 62;   /* largest power of 2 <= 2^63 */
    while (bit) {
        idx_t trial = x + bit;
        if (trial <= n) {
            x = trial;
            n -= trial;
        }
        bit >>= 1;
    }
    return x;
}

/* --- dynamic table of primes --------------------------------------------- */
static idx_t *prime_tbl   = NULL;
static size_t prime_cnt   = 0;
static size_t prime_cap   = 0;

/* Add a prime to the table, growing the array if needed */
static void add_prime(idx_t p)
{
    if (prime_cnt == prime_cap) {
        size_t new_cap = (prime_cap == 0) ? 16 : prime_cap * 2;
        idx_t *new_tbl = (idx_t *)realloc(prime_tbl, new_cap * sizeof(idx_t));
        if (!new_tbl) abort();
        prime_tbl  = new_tbl;
        prime_cap  = new_cap;
    }
    prime_tbl[prime_cnt++] = p;
}

/* Ensure that primes up to at least 'limit' are present in prime_tbl */
static void ensure_primes_upto(idx_t limit)
{
    if (limit < 2) return;
    while (prime_cnt == 0 || prime_tbl[prime_cnt - 1] < limit) {
        idx_t candidate;
        if (prime_cnt == 0) {
            candidate = 2;
        } else {
            candidate = prime_tbl[prime_cnt - 1] + 1;
        }
        if (candidate == 2) {
            add_prime(candidate);
            candidate = 3;
        } else if (candidate % 2 == 0) {
            candidate++;                     /* make odd */
        }
        for (;;) {
            bool is_prime = true;
            idx_t sqrt_c = isqrt_u64(candidate);
            for (size_t i = 0; i < prime_cnt && prime_tbl[i] <= sqrt_c; ++i) {
                if (candidate % prime_tbl[i] == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime) {
                add_prime(candidate);
                break;
            }
            candidate += 2;                  /* skip even numbers */
        }
    }
}

/* Return the n-th prime (n >= 1), using a simple upper bound for n >= 6 */
static idx_t nth_prime(idx_t n)
{
    if (n == 0) return 0;
    /* Upper bound: p_n < n (ln n + ln ln n) for n >= 6, safe for all n. */
    idx_t upper;
    if (n < 6) {
        upper = 15;                          /* covers first 5 primes */
    } else {
        double nd = (double)n;
        double ub = nd * (log(nd) + log(log(nd)));
        upper = (idx_t)(ub + 10.0);          /* small safety margin */
    }
    ensure_primes_upto(upper);
    if (prime_cnt < n) {                      /* rare: increase bound */
        idx_t candidate = prime_tbl[prime_cnt - 1] + 2;
        while (prime_cnt < n) {
            bool is_prime = true;
            idx_t sqrt_c = isqrt_u64(candidate);
            for (size_t i = 0; i < prime_cnt && prime_tbl[i] <= sqrt_c; ++i) {
                if (candidate % prime_tbl[i] == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime) add_prime(candidate);
            candidate += 2;
        }
    }
    return prime_tbl[n - 1];
}

/* Sum of divisors of m (m >= 1) for m fitting in idx_t */
static seq_t sigma_u64(idx_t m)
{
    if (m < 1) return 0;
    seq_t result = 1;
    idx_t temp = m;
    idx_t limit = isqrt_u64(temp);
    ensure_primes_upto(limit);
    for (size_t i = 0; i < prime_cnt && prime_tbl[i] <= temp / prime_tbl[i]; ++i) {
        idx_t p = prime_tbl[i];
        if (temp % p != 0) continue;
        seq_t term = 1;
        seq_t power = 1;
        while (temp % p == 0) {
            temp /= p;
            power *= (seq_t)p;
            term += power;
        }
        result *= term;
    }
    if (temp > 1) {                          /* remaining prime factor */
        seq_t term = 1 + (seq_t)temp;
        result *= term;
    }
    return result;
}

/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined, safe default */
    idx_t p   = nth_prime(n);                  /* p_n */
    idx_t val = p + 1;                         /* p_n + 1 */
    return sigma_u64(val);
}
