/*
 * A096103  Table read by rows: row n contains the quadratic residues modulo n
 *          which are coprime to n.
 * https://oeis.org/A096103
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A096103: flattened table of quadratic residues modulo n that are
   coprime to n.
   Each row n (n ≥ 2) contains the distinct values r = x² mod n with gcd(r,n)=1.
   The sequence a(n) enumerates these rows left‑to‑right, top‑to‑bottom.
   No closed form is known; the implementation computes one row at a time
   and stores the accumulated values in a cache.  */

/* Implementation note: up to n ≈ 10 000 the algorithm runs well below 2 s
   per call.  It uses only integer arithmetic and the standard library.  */

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

/* --- local gcd for 64‑bit integers --------------------------------- */
static inline uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b; b = t;
    }
    return a;
}

/* -- comparator for qsort ------------------------------------------- */
static int seq_cmp(const void *p, const void *q)
{
    seq_t a = *(const seq_t *)p;
    seq_t b = *(const seq_t *)q;
    return (a < b) ? -1 : (a > b);
}

/* --------------------------------------------------------------------- */
/* global cache state                                                     */
static seq_t  *cache       = NULL;   /* flattened sequence values     */
static size_t cache_cap   = 0;       /* allocated capacity (seq_t)    */
static size_t cache_len   = 0;       /* number of values stored       */
static idx_t  next_row    = 2;       /* next n for which row is not yet computed */

/* ensure the cache can store at least `add` additional slots */
static void
ensure_cache_capacity(size_t add)
{
    if (cache_len + add <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap < cache_len + add) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    assert(tmp != NULL);
    cache = tmp;
    cache_cap = new_cap;
}

/* append an array of `cnt` values to the cache */
static void
append_to_cache(const seq_t *src, size_t cnt)
{
    if (cnt == 0) return;
    ensure_cache_capacity(cnt);
    memcpy(cache + cache_len, src, cnt * sizeof(seq_t));
    cache_len += cnt;
}

/* --------------------------------------------------------------------- */
/* main entry point                                                     */
seq_t a(idx_t n)
{
    /* the sequence starts at index 2 */
    if (n < 2) return (seq_t)0;

    /* grow the cache until it contains a(n) */
    while (cache_len < (size_t)(n - 1)) {
        idx_t mod = next_row;                     /* current row index n */
        size_t msize = (size_t)mod;               /* size of visited array */

        bool *visited = (bool *)calloc(msize, sizeof(bool));
        assert(visited != NULL);

        seq_t *row = (seq_t *)malloc(msize * sizeof(seq_t));
        assert(row != NULL);
        size_t count = 0;

        uint64_t mod64 = (uint64_t)mod;

        for (idx_t x = 0; x < mod; ++x) {
            uint64_t r64 = ((uint64_t)x * x) % mod64;
            if (gcd_uint64(r64, mod64) == 1) {
                size_t rx = (size_t)r64;          /* r64 < mod ≤ 10 000 */
                if (!visited[rx]) {
                    visited[rx] = true;
                    row[count++] = (seq_t)(r64 & 0xFFFFFFFFFFFFFFFFULL); /* cast to seq_t */
                }
            }
        }

        /* sort the distinct residues */
        if (count > 1) qsort(row, count, sizeof(seq_t), seq_cmp);

        append_to_cache(row, count);

        free(row);
        free(visited);
        ++next_row;
    }

    return cache[n - 2];   /* a(2) is at cache[0] */
}
