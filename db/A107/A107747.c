/*
 * A107747  Numbers k such that the least prime factor of 6*k+1 < the least
 *          prime factor of 6*k-1, A107744(k) < A107745(k).
 * https://oeis.org/A107747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A107747
 * Numbers k such that the least prime factor of 6*k+1 is less than the
 * least prime factor of 6*k-1.
 * The implementation builds the sequence incrementally, using a
 * dynamic list of primes and a simple trial division routine for
 * least–prime–factor calculation.  The first term is a(1)=4.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- auxiliary prime table ---------- */
static uint32_t *prime_tbl = NULL;   /* array of primes */
static size_t prime_cnt  = 0;         /* number of primes stored */
static size_t prime_cap  = 0;         /* allocated capacity */
static uint32_t prime_last_checked = 2; /* last integer checked for primality */

/* expand prime table capacity */
static void extend_prime_tbl(void)
{
    size_t newcap = prime_cap == 0 ? 16 : prime_cap * 2;
    uint32_t *tmp = (uint32_t *)realloc(prime_tbl, newcap * sizeof(uint32_t));
    if (!tmp) abort();
    prime_tbl = tmp;
    prime_cap = newcap;
}

/* add prime to table (assuming prime_tbl has enough space) */
static void add_prime(uint32_t p)
{
    if (prime_cnt == prime_cap) extend_prime_tbl();
    prime_tbl[prime_cnt++] = p;
}

/* integer sqrt of a uint64_t, floor, using 32 iterations of the
 * binary–digit method.  Works for all x < 2^64. */
static inline uint32_t isqrt_u64(uint64_t x)
{
    uint64_t res = 0;
    for (int i = 31; i >= 0; --i) {
        uint64_t bit = 1ULL << i;
        if ((res + bit) * (res + bit) <= x) res += bit;
    }
    return (uint32_t)res;
}

/* ensure that all primes up to 'limit' are in prime_tbl */
static void ensure_primes(uint32_t limit)
{
    if (limit <= prime_last_checked) return;

    for (uint32_t n = prime_last_checked + 1; n <= limit; ++n) {
        bool isprime = true;
        uint32_t r = isqrt_u64(n);
        for (size_t i = 0; i < prime_cnt && prime_tbl[i] <= r; ++i) {
            if (n % prime_tbl[i] == 0) { isprime = false; break; }
        }
        if (isprime) add_prime(n);
    }
    prime_last_checked = limit;
}

/* least prime factor of n (n >= 2).  Uses prime_tbl for trial division. */
static uint64_t least_prime_factor(uint64_t n)
{
    if (n <= 1) return n;
    uint32_t lim = isqrt_u64(n);
    ensure_primes(lim);
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_tbl[i];
        if ((uint64_t)p * (uint64_t)p > n) break;
        if (n % p == 0) return p;
    }
    return n;                /* n is prime */
}

/* ---------- sequence storage ---------- */
static seq_t *seq_vals = NULL;     /* array of terms a(i) */
static idx_t seq_size = 0;        /* number of terms stored */
static idx_t seq_cap  = 0;        /* allocated capacity */

/* expand sequence storage */
static void extend_seq_vals(void)
{
    size_t newcap = seq_cap == 0 ? 16 : seq_cap * 2;
    seq_t *tmp = (seq_t *)realloc(seq_vals, newcap * sizeof(seq_t));
    if (!tmp) abort();
    seq_vals = tmp;
    seq_cap = newcap;
}

/* first k to test (value of k, not index in sequence) */
static idx_t next_k = 1;

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    /* initialize prime table with first prime */
    if (prime_cnt == 0) add_prime(2);

    while ((idx_t)seq_size < n) {
        /* compute least prime factors of 6*next_k ± 1 */
        uint64_t v1 = 6ULL * (uint64_t)next_k + 1ULL;
        uint64_t v2 = 6ULL * (uint64_t)next_k - 1ULL;
        uint64_t lpf1 = least_prime_factor(v1);
        uint64_t lpf2 = least_prime_factor(v2);

        if (lpf1 < lpf2) {
            if (seq_size == seq_cap) extend_seq_vals();
            seq_vals[seq_size++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return seq_vals[n - 1];
}
