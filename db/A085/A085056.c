/*
 * A085056  (Product of all composite numbers from 1 to n)/ ( product of the
 *          prime divisors of all composite numbers up to n). More precisely,
 *          denominator = product of the largest squarefree divisors of
 *          composite numbers up to n.
 * https://oeis.org/A085056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{i=2..n} ( i / rad(i) ), where rad(i) is the
 *            product of distinct prime factors of i. (For primes and 1, the
 *            factor equals 1.) Recurrence: a(1)=1, a(n)=a(n-1)*(n/rad(n))
 *            (rad(n)=1 if n is prime)
 * Range:     u8 n <= 15, u16 n <= 26, u32 n <= 47, u64 n <= 80, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- prime generation ------------------------------------------------ */
static uint32_t *primes = NULL;     /* array of primes, 32‑bit sufficient for n ≤ 2³¹-1 */
static size_t   primes_cnt = 0;     /* how many primes currently stored */
static size_t   primes_alloc = 0;   /* allocated capacity (bytes) */
static idx_t   primes_limit = 0;    /* largest n for which primes are known */

/* Ensure that primes[0..primes_cnt-1] contains all primes ≤ limit. */
static void ensure_primes(idx_t limit)
{
    if (limit <= primes_limit) return;

    /* Allocate temporary boolean array for Sieve of Eratosthenes */
    size_t sz = (size_t)limit + 1;
    bool *is_comp = (bool*)calloc(sz, sizeof(bool));
    if (!is_comp) abort();

    /* Reset primes array */
    primes_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            /* grow primes array if needed */
            if (primes_cnt == primes_alloc) {
                size_t new_alloc = primes_alloc ? primes_alloc * 2 : 16;
                uint32_t *tmp = (uint32_t*)realloc(primes, new_alloc * sizeof(uint32_t));
                if (!tmp) abort();
                primes = tmp;
                primes_alloc = new_alloc;
            }
            primes[primes_cnt++] = (uint32_t)i;

            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i) {
                    is_comp[j] = true;
                }
            }
        }
    }
    free(is_comp);
    primes_limit = limit;
}

/* Compute the factor n / rad(n).  For primes and 1 this returns 1. */
static seq_t factor_n(idx_t n)
{
    uint64_t rad = 1;
    uint64_t m = n;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        if (pp > m) break;
        if (m % p == 0) {
            rad *= p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) rad *= m;          /* remaining prime > sqrt(n) */
    return (seq_t)(n / rad);
}

/* ---------- sequence storage ---------------------------------------------- */
static seq_t *a_vals = NULL;   /* array of a(n) values, index 0..cap-1 */
static size_t  a_alloc = 0;    /* allocated capacity of a_vals */
static idx_t   a_max = 0;      /* largest n for which a_vals has been computed */

/* Ensure that a_vals can hold indices up to n (inclusive). */
static void ensure_a_vals(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (a_alloc < needed) {
        size_t new_cap = a_alloc > 0 ? a_alloc : 16;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(a_vals, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        a_vals = tmp;
        a_alloc = new_cap;
    }
}

/* ---------- required public function --------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* sequence starts at a(1) */

    ensure_primes(n);                  /* prime list for factorisation */
    ensure_a_vals(n);                  /* storage for a(1..n) */

    if (a_max == 0) {                  /* first initialisation */
        a_vals[1] = (seq_t)1;
        a_max = 1;
    }

    for (idx_t i = a_max + 1; i <= n; ++i) {
        seq_t f = factor_n(i);
        a_vals[i] = a_vals[i - 1] * f;
    }
    a_max = n;
    return a_vals[n];
}
