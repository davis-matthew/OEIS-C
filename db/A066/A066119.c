/*
 * A066119  Multi-level primorials: triangle with a(n,k)=a(n-1,k-1)*a(n-1,k)
 *          but with a(n,1)=p(n) and a(n,n)=2.
 * https://oeis.org/A066119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 17, u32 n <= 30, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS A066119 – Multi‑level primorial
   triangle read row by row.
   Recurrence:
     A(1,1)     = 2
     A(n,1)     = p(n)            (n‑th prime)
     A(n,n)     = 2
     A(n,k)     = A(n-1,k-1) * A(n-1,k)   (1 < k < n)
   Flattened sequence: a(n) = A(1,1), A(2,1), A(2,2), A(3,1), ….
   a(1)=2, a(2)=3, a(3)=2, a(4)=5, a(5)=6, a(6)=2, … */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* ---------- integer square root (floor) ---------- */
static inline idx_t int_isqrt(idx_t n)
{
    idx_t r = 0;
    idx_t bit = (idx_t)1ULL << 62;             /* highest power of 4 <= 2^64 */
    while (bit > n) bit >>= 2;
    while (bit) {
        if (n >= r + bit) {
            n -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* ---------- prime utilities ---------- */
static uint32_t *prime_list = NULL;   /* array of primes found so far */
static size_t   prime_count = 0;
static idx_t    prime_candidate = 2;   /* next candidate to test */

/* test whether candidate is prime using the primes already found */
static int is_prime_candidate(uint64_t candidate)
{
    if (candidate < 2) return 0;
    idx_t limit = int_isqrt(candidate);
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if ((idx_t)p > limit) break;
        if (candidate % p == 0) return 0;
    }
    return 1;
}

/* ensure that prime_list contains at least k primes (k >= 1) */
static void ensure_prime_count(idx_t k)
{
    while (prime_count < (size_t)k) {
        if (is_prime_candidate(prime_candidate)) {
            size_t new_cap = prime_count ? prime_count * 2 : 8;
            if (prime_count + 1 > new_cap) new_cap = prime_count + 1;
            uint32_t *tmp = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
            if (!tmp) abort();
            prime_list = tmp;
            prime_list[prime_count++] = (uint32_t)prime_candidate;
        }
        ++prime_candidate;
    }
}

/* ---------- triangle cache ---------- */
static seq_t *cache      = NULL;   /* flattened sequence a(1), a(2), …          */
static size_t cache_len  = 0;       /* number of elements cached                */
static size_t cache_cap  = 0;       /* capacity of cache array                  */

static seq_t *prev_row   = NULL;   /* array holding the previous row            */
static idx_t  prev_len   = 0;       /* length of previous row                    */

/* generate rows until cache contains at least 'target' elements */
static void ensure_cache_size(idx_t target)
{
    while (cache_len < (size_t)target) {
        idx_t row = prev_len + 1;   /* next row number (1‑based) */
        idx_t len = row;           /* number of elements in this row */

        seq_t *row_buf = (seq_t *)malloc(sizeof(seq_t) * len);
        if (!row_buf) abort();

        /* first element = p(row) */
        ensure_prime_count(row);
        row_buf[0] = (seq_t)prime_list[row - 1];

        /* interior elements */
        for (idx_t k = 1; k + 1 < len; ++k) {
            row_buf[k] = prev_row[k - 1] * prev_row[k];
        }

        /* last element */
        row_buf[len - 1] = (seq_t)2;

        /* append row_buf to cache */
        size_t needed = cache_len + len;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            while (new_cap < needed) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                free(row_buf);
                abort();
            }
            cache = tmp;
            cache_cap = new_cap;
        }
        for (size_t i = 0; i < (size_t)len; ++i) {
            cache[cache_len++] = row_buf[i];
        }

        /* keep the new row as previous for next iteration */
        free(prev_row);
        prev_row = row_buf;
        prev_len = len;
    }
}

/* ---------- public entry point ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for n < 1 */

    /* initialize first row if needed */
    if (cache_len == 0) {
        prev_row = (seq_t *)malloc(sizeof(seq_t) * 1);
        if (!prev_row) abort();
        prev_row[0] = (seq_t)2;
        prev_len = 1;

        cache_cap = 8;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) abort();
        cache[0] = (seq_t)2;
        cache_len = 1;
    }

    if (cache_len < (size_t)n) {
        ensure_cache_size(n);
    }

    return cache[n - 1];
}
