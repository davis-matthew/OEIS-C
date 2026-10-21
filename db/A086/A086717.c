/*
 * A086717  Convolution of primes with partition numbers.
 * https://oeis.org/A086717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 83, u64 n <= 313,
 *            u128 n <= 1221, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A086717
 * a(n) = Sum_{k=1..n} A000041(k)*A000040(n+1-k)
 * where A000041(k) = partition number p(k)
 * and A000040(m) = m-th prime
 *
 * Implements a(n) with caching of partition numbers and primes.
 * Supports any integer type seq_t and idx_t (unsigned 64-bit).
 * Uses only standard C libraries. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- static data ---------------------------------- */

static seq_t *p_cache = NULL;        /* partition numbers p[0..max]   */
static idx_t  p_cached = 0;          /* largest index cached           */
static uint32_t *prime_cache = NULL; /* primes[0..cnt-1]              */
static idx_t    prime_cnt = 0;       /* number of primes cached        */
static idx_t    prime_cap = 0;       /* allocated capacity              */
static idx_t    prime_next = 2;      /* next integer to test for prime */


/* ---------- utility: ensure primes up to index n ---------- */

static void ensure_primes(idx_t n)
{
    /* nothing to do if already have at least n primes */
    if (prime_cnt >= n) return;

    /* grow storage if necessary */
    if (prime_cap == 0) {
        prime_cap = (n < 16 ? 16 : n);
        prime_cache = (uint32_t *)malloc(sizeof(uint32_t) * prime_cap);
        if (!prime_cache) abort();
    }
    while (prime_cnt < n) {
        if (prime_cap == prime_cnt) {
            /* double capacity */
            prime_cap <<= 1;
            uint32_t *tmp = (uint32_t *)realloc(prime_cache, sizeof(uint32_t) * prime_cap);
            if (!tmp) abort();
            prime_cache = tmp;
        }

        /* test candidate prime_next */
        bool is_prime = true;
        for (idx_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = prime_cache[i];
            if ((idx_t)p * p > prime_next) break;
            if (prime_next % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            prime_cache[prime_cnt++] = (uint32_t)prime_next;
        }
        ++prime_next;
    }
}

/* ---------- utility: ensure partition numbers up to n ------ */

static void ensure_partitions(idx_t n)
{
    if (p_cached >= n) return;

    /* expand cache array */
    if (p_cache == NULL) {
        p_cache = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
        if (!p_cache) abort();
    } else if (p_cached + 1 <= n) {
        seq_t *tmp = (seq_t *)realloc(p_cache, sizeof(seq_t) * (n + 1));
        if (!tmp) abort();
        p_cache = tmp;
    }

    /* p(0) = 1 */
    if (p_cached == 0) p_cache[0] = (seq_t)1;

    /* compute p from p_cached+1 up to n */
    for (idx_t m = p_cached + 1; m <= n; ++m) {
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1; ; ++k) {
            /* generalized pentagonal numbers */
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (m < pent1) break;
            idx_t term1 = m - pent1;
            if (k & 1)
                pos += p_cache[term1];
            else
                neg += p_cache[term1];

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (m < pent2) continue;
            idx_t term2 = m - pent2;
            if (k & 1)
                pos += p_cache[term2];
            else
                neg += p_cache[term2];
        }
        p_cache[m] = pos - neg;   /* pos >= neg */
    }
    p_cached = n;
}

/* ---------- the sequence function -------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* ignore indices below the offset */
    ensure_partitions(n);
    ensure_primes(n);                /* need primes 1..n */

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        /* prime index: n+1-k  -> array index: (n+1-k)-1 = n-k */
        seq_t term = p_cache[k] * (seq_t)prime_cache[n - k];
        sum += term;
    }
    return sum;
}
