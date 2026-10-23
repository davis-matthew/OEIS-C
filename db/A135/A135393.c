/*
 * A135393  Not the sum of three perfect powers greater than 1.
 * https://oeis.org/A135393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer that cannot be expressed as a
 *            sum of three perfect powers greater than 1 (m^k with m > 1, k ≥
 *            2)
 * Range:     u8 n <= 26, u16 n <= 34, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Static state for caching computed terms and for perfect‑power data. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_size = 0;
static idx_t  last_checked = 0;               /* largest integer processed */

static seq_t *powers     = NULL;              /* sorted list of perfect powers */
static size_t powers_len   = 0;
static size_t powers_cap   = 0;
static bool *is_pow        = NULL;            /* boolean table of perfect powers */
static idx_t  pow_limit    = 0;                /* maximum value covered by is_pow */

/* Helper: comparison function for qsort on seq_t. */
static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *pa = (const seq_t *)a;
    const seq_t *pb = (const seq_t *)b;
    if (*pa < *pb) return -1;
    if (*pa > *pb) return 1;
    return 0;
}

/* Ensure the cache has room for at least n entries. */
static void grow_cache(void)
{
    if (cache_size == cache_cap) {
        cache_cap = (cache_cap ? cache_cap * 2 : 8);
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(*cache));
    }
}

/* Expand the is_pow table to cover values up to 'limit' and generate
   all perfect powers ≤ limit.  Only adds entries beyond the current
   pow_limit. */
static void ensure_powers(idx_t limit)
{
    if (limit <= pow_limit) return;

    /* Expand the bool array */
    size_t new_cap = (size_t)limit + 1;               /* +1 to include index == limit */
    bool *new_is_pow = (bool *)realloc(is_pow, new_cap);
    if (!new_is_pow) abort();                        /* allocation failure */
    if (new_is_pow != is_pow) is_pow = new_is_pow;
    if (pow_limit + 1 < new_cap) {
        memset(is_pow + pow_limit + 1,
               0,
               (new_cap - pow_limit - 1) * sizeof(bool));
    }

    /* Generate perfect powers between (pow_limit, limit] */
    for (idx_t base = 2; base * base <= limit; ++base) {
        seq_t val = (seq_t)base * base;
        while (val <= limit) {
            size_t idx = (size_t)val;
            if (!is_pow[idx]) {
                is_pow[idx] = true;
                /* Append to powers list */
                if (powers_len == powers_cap) {
                    powers_cap = (powers_cap ? powers_cap * 2 : 8);
                    powers = (seq_t *)realloc(powers,
                                             powers_cap * sizeof(seq_t));
                }
                powers[powers_len++] = val;
            }
            /* Prevent overflow of val */
            if (val > (seq_t)limit / base) break;
            val *= base;
        }
    }

    /* Sort the powers list for efficient searching */
    if (powers_len > 1) {
        qsort(powers, powers_len, sizeof(seq_t), cmp_seq_t);
    }
    pow_limit = limit;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* value for n=0 is undefined */

    /* Keep generating terms until we have at least n cached. */
    while (cache_size < n) {
        idx_t candidate = last_checked + 1; /* next integer to test */
        ensure_powers(candidate);          /* guarantee perfect powers up to this value */

        /* Check if candidate can be written as a+b+c with perfect powers. */
        bool representable = false;
        seq_t seq_pow_limit = (seq_t)pow_limit;

        for (size_t i = 0; i < powers_len && !representable; ++i) {
            seq_t p1 = powers[i];
            if (p1 > candidate) break;
            for (size_t j = i; j < powers_len; ++j) {
                seq_t p2 = powers[j];
                seq_t sum2 = p1 + p2;
                if (sum2 > candidate) break;
                seq_t c = candidate - sum2;
                if (c < p2) break;      /* enforce p1 <= p2 <= c */

                if (c <= seq_pow_limit) {
                    idx_t ci = (idx_t)c;   /* safe: c <= pow_limit <= idx_t max */
                    if (is_pow[ci]) {
                        representable = true;
                        break;
                    }
                }
            }
        }

        if (!representable) {
            grow_cache();
            cache[cache_size++] = (seq_t)candidate;
        }
        last_checked = candidate;
    }

    return cache[n - 1];
}
