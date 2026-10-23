/*
 * A154722  Triangle read by rows in which row n lists: n, in the center of
 *          the row and the pairs of noncomposite numbers that are equidistant
 *          to n, as shown below in the example.
 * https://oeis.org/A154722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 1930, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A154722: the triangle read by rows, where row n contains
   n in the center and all pairs of non-composite numbers that are
   equidistant from n.  The function builds the triangle lazily and
   caches each computed term.  It uses a simple prime sieve up to
   twice the current row number. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static unsigned char *prime_composite = NULL;   /* 0 = non‑composite (prime or 1), 1 = composite */
static idx_t prime_limit = 0;                   /* current maximum index for prime_composite */

static void ensure_prime_limit(idx_t limit) {
    if (prime_limit >= limit) return;
    idx_t new_limit = limit;
    unsigned char *arr = (unsigned char *)calloc(new_limit + 1, 1);
    if (!arr) exit(1);                       /* allocation failure */
    for (idx_t p = 2; p * p <= new_limit; ++p)
        if (!arr[p])
            for (idx_t j = p * p; j <= new_limit; j += p)
                arr[j] = 1;                 /* mark composite */
    free(prime_composite);
    prime_composite = arr;
    prime_limit = new_limit;
}

static inline bool is_noncomposite(idx_t n) {
    if (n == 1) return true;                /* 1 is considered non‑composite */
    return prime_composite[n] == 0;
}

/* Cache for the sequence values. */
static seq_t *seq_cache = NULL;
static size_t seq_cache_len = 0;            /* number of values stored */
static size_t seq_cache_cap = 0;            /* capacity of seq_cache */

/* Ensure seq_cache capacity for at least 'need' elements. */
static void ensure_cache_capacity(size_t need) {
    if (seq_cache_cap >= need) return;
    size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 64;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_ptr) exit(1);
    seq_cache = new_ptr;
    seq_cache_cap = new_cap;
}

/* Temporary storage for left / right sides of a row. */
static idx_t *row_l = NULL;
static idx_t *row_r = NULL;
static size_t row_cap = 0;                  /* capacity of row_l / row_r */

/* Ensure sufficient capacity for row side buffers. */
static void ensure_row_cap(size_t need) {
    if (row_cap >= need) return;
    size_t new_cap = row_cap ? row_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    idx_t *new_l = (idx_t *)realloc(row_l, new_cap * sizeof(idx_t));
    idx_t *new_r = (idx_t *)realloc(row_r, new_cap * sizeof(idx_t));
    if (!new_l || !new_r) exit(1);
    row_l = new_l;
    row_r = new_r;
    row_cap = new_cap;
}

/* Number of rows already computed. */
static idx_t rows_computed = 0;

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;                   /* out of range guard */

    /* Compute rows until the cache contains at least n elements. */
    while (seq_cache_len < (size_t)n) {
        idx_t r = rows_computed + 1;        /* next row to compute */
        ensure_prime_limit(2 * r);          /* need primes up to 2*r */

        /* Find all non‑composite pairs equidistant from r. */
        ensure_row_cap((size_t)r);
        size_t left_cnt = 0, right_cnt = 0;

        for (idx_t d = 1; d < r; ++d) {
            idx_t l = r - d;
            idx_t rt = r + d;
            if (is_noncomposite(l) && is_noncomposite(rt)) {
                row_l[left_cnt++] = l;
                row_r[right_cnt++] = rt;
            }
        }

        /* Append the row into the cache: left side (reversed), center, right side. */
        size_t needed = left_cnt + 1 + right_cnt;
        ensure_cache_capacity(seq_cache_len + needed);

        for (size_t i = 0; i < left_cnt; ++i)
            seq_cache[seq_cache_len++] = (seq_t)row_l[left_cnt - 1 - i];
        seq_cache[seq_cache_len++] = (seq_t)r;
        for (size_t i = 0; i < right_cnt; ++i)
            seq_cache[seq_cache_len++] = (seq_t)row_r[i];

        rows_computed = r;
    }

    return seq_cache[(size_t)n - 1];
}
