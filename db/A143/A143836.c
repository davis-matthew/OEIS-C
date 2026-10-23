/*
 * A143836  Triangle read by rows: T(r,c) = (prime(r+2) + prime(c+1))/2.
 * https://oeis.org/A143836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows: T(r,c) = (prime(r+2) + prime(c+1)) / 2
   a(n) is the linear index value in the triangle (indexing starts at 1). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ---- Prime cache ---- */
static uint64_t *prime_tbl = NULL;  /* array of primes as uint64_t */
static size_t       prime_cnt = 0;
static size_t       prime_cap = 0;

static void prime_grow(size_t need)
{
    if (prime_cap >= need) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(prime_tbl, new_cap * sizeof(uint64_t));
    prime_tbl = tmp;
    prime_cap = new_cap;
}

static void prime_add(uint64_t p)
{
    prime_grow(prime_cnt + 1);
    prime_tbl[prime_cnt++] = p;
}

/* Test whether x is prime using the primes already stored. */
static int is_prime_candidate(uint64_t x)
{
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_tbl[i];
        if (p > x / p) break;           /* p*p > x */
        if (x % p == 0) return 0;
    }
    return 1;
}

/* Ensure the prime cache contains at least 'need' primes. */
static void prime_ensure(size_t need)
{
    if (prime_cnt >= need) return;
    if (prime_cnt == 0) prime_add(2);   /* start with 2 */
    uint64_t cand = prime_tbl[prime_cnt-1] + 1;
    if ((cand & 1) == 0) cand++;        /* make odd if necessary */
    while (prime_cnt < need) {
        if (is_prime_candidate(cand)) prime_add(cand);
        cand += 2;
    }
}

/* Find smallest r such that r(r+1)/2 >= n. */
static idx_t row_of(idx_t n)
{
    idx_t lo = 1, hi = 1;
    /* Increase hi until triangular(hi) >= n */
    while ((((unsigned __int128)hi) * (hi + 1)) / 2 < n) {
        hi <<= 1;
    }
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if ((((unsigned __int128)mid) * (mid + 1)) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* Main function required by the harness. */
seq_t a(idx_t n)
{
    /* Determine row and column (1‑based) from the linear index. */
    idx_t r = row_of(n);
    idx_t prev_sum = ((idx_t)(r - 1) * r) / 2;  /* elements before row r */
    idx_t c = n - prev_sum;                    /* column in the row */

    /* Ensure we have enough primes:
       need up to max(r+2, c+1). */
    size_t need = (r + 2) > (c + 1) ? (r + 2) : (c + 1);
    prime_ensure(need);

    /* Retrieve the required primes. */
    seq_t p1 = (seq_t)prime_tbl[r + 1];  /* (r+2)-th prime */
    seq_t p2 = (seq_t)prime_tbl[c];      /* (c+1)-th prime */

    /* Compute (p1 + p2)/2. */
    return (p1 + p2) / (seq_t)2;
}
