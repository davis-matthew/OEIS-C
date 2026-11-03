/*
 * A174349  Square array: row n gives the indices i for which prime(i+1) =
 *          prime(i) + 2n; read by falling antidiagonals.
 * https://oeis.org/A174349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 940, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Validated: standard, pass, 82 terms through n = 1216 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A174349: indices i for which the gap between successive primes
   equals 2r are arranged in a square array; the array is read by falling
   antidiagonals. This implementation builds the necessary prime gaps
   incrementally and answers queries in any order. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

struct Row {
    idx_t *data;
    size_t len;
    size_t cap;
};

static struct Row *rows = NULL;
static size_t rows_cap = 0;
static idx_t *primes = NULL;
static size_t primes_cap = 0;
static size_t primes_len = 0;

/* Ensure the rows array can store at least indices up to r (zero‑based). */
static void ensure_rows_cap(size_t r)
{
    if (r >= rows_cap) {
        size_t new_cap = rows_cap ? rows_cap * 2 : 8;
        while (new_cap <= r) new_cap *= 2;
        struct Row *tmp = (struct Row *)realloc(rows, new_cap * sizeof(struct Row));
        rows = tmp;
        for (size_t i = rows_cap; i < new_cap; ++i) {
            rows[i].data = NULL;
            rows[i].len  = 0;
            rows[i].cap  = 0;
        }
        rows_cap = new_cap;
    }
}

/* Append a value to row r. */
static void add_to_row(size_t r, idx_t val)
{
    struct Row *row = &rows[r];
    if (row->len == row->cap) {
        size_t new_cap = row->cap ? row->cap * 2 : 4;
        idx_t *tmp = (idx_t *)realloc(row->data, new_cap * sizeof(idx_t));
        row->data = tmp;
        row->cap  = new_cap;
    }
    row->data[row->len++] = val;
}

/* Generate the next prime, update arrays, and record prime gaps. */
static void generate_next_prime(void)
{
    /* get_prime uses 1‑based indexing for prime numbers */
    uint32_t next = get_prime((idx_t)(primes_len + 1));
    if (primes_len == primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 8;
        idx_t *tmp = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
        primes = tmp;
        primes_cap = new_cap;
    }
    primes[primes_len++] = (idx_t)next;

    if (primes_len >= 2) {
        idx_t diff = primes[primes_len - 1] - primes[primes_len - 2];
        if ((diff & 1ULL) == 0) {            /* even difference */
            idx_t rdiff = diff >> 1;         /* r = diff / 2 */
            ensure_rows_cap((size_t)rdiff);
            /* The smaller prime's 1‑based index is i = primes_len - 1 */
            idx_t i = (idx_t)(primes_len - 1);
            add_to_row((size_t)rdiff, i);
        }
    }
}

/* Make sure row r contains at least c elements (1‑based counting). */
static void ensure_row_has(idx_t r, idx_t c)
{
    if (r == 0) return;   /* safety, r is never 0 in valid calls */
    ensure_rows_cap((size_t)r);
    while (rows[(size_t)r].len < (size_t)c) {
        generate_next_prime();
    }
}

/* Convert n to falling‑antidiagonal coordinates (r, c).  Both are 1‑based. */
static void antidiagonal_coords(idx_t n, idx_t *r_out, idx_t *c_out)
{
    /* Find minimal s such that s*(s+1)/2 >= n;
       s = d-1 where d is the diagonal sum (r+c). */
    idx_t lo = 0, hi = 1;
    while ( ( (__int128)hi * (hi + 1) ) / 2 < n) hi <<= 1;
    while (lo + 1 < hi) {
        idx_t mid = (lo + hi) >> 1;
        if ( ( (__int128)mid * (mid + 1) ) / 2 >= n )
            hi = mid;
        else
            lo = mid;
    }
    idx_t s = hi;            /* minimal s with triangular >= n */
    idx_t d = s + 1;         /* diagonal sum r + c */
    idx_t prev = (d - 2) * (d - 1) / 2;   /* triangular of d-2 */
    idx_t k = n - prev;                  /* offset within diagonal, 1‑based */
    *r_out = k;             /* row index */
    *c_out = d - k;         /* column index */
}

/* Sequence A174349 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* undefined, safety */
    idx_t r, c;
    antidiagonal_coords(n, &r, &c);
    ensure_row_has(r, c);
    idx_t value = rows[(size_t)r].data[(size_t)c - 1];
    return (seq_t)value;
}
