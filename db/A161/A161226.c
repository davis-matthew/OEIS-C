/*
 * A161226  a(0)=0. a(n) = the smallest integer of the form k^j, j>=2, such
 *          that a(n) >= a(n-1) + n.
 * https://oeis.org/A161226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A161226: a(0)=0, a(n) = smallest perfect power k^j (j>=2)
   such that a(n) >= a(n-1) + n. */
#include <stdlib.h>
#include <stdint.h>

/* comparison for qsort and binary search */
static int compare_seq_t(const void *p1, const void *p2)
{
    seq_t v1 = *(const seq_t *)p1;
    seq_t v2 = *(const seq_t *)p2;
    if (v1 < v2) return -1;
    if (v1 > v2) return 1;
    return 0;
}

/* precomputed list of perfect powers <= 200 000 000 that fit in seq_t */
static seq_t *powers = NULL;
static size_t p_size = 0, p_cap = 0;
static int powers_initialized = 0;

static void init_powers(void)
{
    if (powers_initialized) return;
    powers_initialized = 1;
    const uint64_t LIMIT = 200000000ULL;
    /* maximum value that seq_t can hold (for 64‑bit and smaller types) */
    const uint64_t seq_max = (uint64_t)~(seq_t)0;

    p_cap = 1024;
    powers = (seq_t *)malloc(p_cap * sizeof(seq_t));
    /* generate all powers k^j >= 2, j>=2, base >= 2 */
    for (uint64_t base = 2; base * base <= LIMIT; ++base) {
        uint64_t val = base * base;             /* base^2 */
        while (val <= LIMIT) {
            if (val <= seq_max) {
                if (p_size == p_cap) {
                    p_cap <<= 1;
                    powers = (seq_t *)realloc(powers, p_cap * sizeof(seq_t));
                }
                powers[p_size++] = (seq_t)val;
            }
            if (val > LIMIT / base) break;      /* prevent overflow */
            val *= base;
        }
    }
    /* include 1 (all exponents give 1) */
    if (seq_max >= 1) {
        if (p_size == p_cap) {
            p_cap <<= 1;
            powers = (seq_t *)realloc(powers, p_cap * sizeof(seq_t));
        }
        powers[p_size++] = (seq_t)1;
    }

    /* sort and remove duplicates */
    qsort(powers, p_size, sizeof(seq_t), compare_seq_t);
    size_t new_len = 0;
    for (size_t i = 0; i < p_size; ++i) {
        if (new_len == 0 || powers[i] != powers[new_len - 1]) {
            powers[new_len++] = powers[i];
        }
    }
    p_size = new_len;
}

/* cache of computed sequence values a(n) */
static seq_t *cache = NULL;
static size_t c_size = 0, c_cap = 0;

extern seq_t a(idx_t n)
{
    init_powers();
    if (cache == NULL) {
        c_cap = 16;
        cache = (seq_t *)malloc(c_cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        c_size = 1;
    }

    /* compute up to index n */
    while (c_size <= n) {
        seq_t prev = cache[c_size - 1];
        idx_t  idx  = (idx_t)c_size;                 /* current n value */
        seq_t threshold = prev + (seq_t)idx;

        /* binary search in powers for first element >= threshold */
        size_t lo = 0, hi = p_size;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (powers[mid] < threshold) {
                lo = mid + 1;
            } else {
                hi = mid;
            }
        }
        seq_t next = powers[lo];

        if (c_size == c_cap) {
            c_cap <<= 1;
            cache = (seq_t *)realloc(cache, c_cap * sizeof(seq_t));
        }
        cache[c_size++] = next;
    }

    return cache[n];
}
