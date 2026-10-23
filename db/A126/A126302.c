/*
 * A126302  a(n) = Sum of peak heights of the n-th Dyck-path encoded by
 *          A014486(n).
 * https://oeis.org/A126302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum of peak heights of the n-th Dyck-path encoded by
 *            A014486(n)
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   peak_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: Enumerate Dyck paths in ascending numeric order of their binary
   encoding (1 for up, 0 for down) and compute the sum of peak heights for each.   */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/peak_sum.h"

/* ---------- Global static state ---------- */
static seq_t *cache = NULL;        /* cache of computed a(n), index 0 holds a(1)          */
static size_t cache_size = 0;      /* number of terms currently stored in cache           */
static size_t cache_cap   = 0;      /* current capacity of cache                          */
static idx_t  current_k   = 0;      /* maximum semilength of Dyck paths cached             */
static idx_t  cumulative  = 0;      /* total number of Dyck paths cached so far            */
static unsigned long long catalan[25]; /* Catalan numbers C[0]..C[24]                      */

/* ---------- Forward declarations ---------- */
static void push_cache(seq_t val);
static void rec_generate(idx_t k, idx_t pos, int height,
                         idx_t ups, idx_t downs, int prev_bit,
                         uint64_t code);
static seq_t peak_sum(uint64_t code, int len);

/* ---------- Helper: push value into cache ---------- */
static void push_cache(seq_t val)
{
    if (cache_cap == 0) {
        cache_cap = 256;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_size >= cache_cap) {
        cache_cap <<= 1;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_size++] = val;
}

/* ---------- Recursive generator for Dyck paths of semilength k ---------- */
static void rec_generate(idx_t k, idx_t pos, int height,
                         idx_t ups, idx_t downs, int prev_bit,
                         uint64_t code)
{
    if (pos == 2 * k) {
        push_cache(peak_sum(code, (int)(2 * k)));
        return;
    }

    /* Try adding a down step (bit 0) first for ascending numeric order */
    if (downs > 0 && height > 0) {
        rec_generate(k, pos + 1, height - 1,
                     ups, downs - 1, 0,
                     code << 1);
    }

    /* Then try adding an up step (bit 1) */
    if (ups > 0) {
        rec_generate(k, pos + 1, height + 1,
                     ups - 1, downs, 1,
                     (code << 1) | 1ULL);
    }
}

/* ---------- Public function required by the sequence interface ---------- */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;                     /* offset 0 */
    }

    /* Initialise Catalan numbers once */
    static int catalan_initialized = 0;
    if (!catalan_initialized) {
        catalan[0] = 1ULL;
        for (int i = 1; i < 25; ++i) {
            unsigned __int128 num = (unsigned __int128)catalan[i - 1] * 2ULL * (2ULL * i - 1ULL);
            num /= (i + 1ULL);
            catalan[i] = (unsigned long long)num;
        }
        catalan_initialized = 1;
    }

    /* Generate additional Dyck paths until we have at least n terms */
    while (cumulative < n) {
        ++current_k;                     /* Next semilength */
        cumulative += catalan[current_k];
        /* Generate all Dyck paths of semilength current_k */
        rec_generate(current_k,
                     1,                      /* start after first up step */
                     1,                      /* current height */
                     current_k - 1,          /* ups remaining (excluding first up) */
                     current_k,              /* downs remaining */
                     1,                      /* previous bit is 1 (first up) */
                     1ULL);                  /* code with first bit 1 */
    }

    /* Return the cached result for a(n) */
    return cache[n - 1];
}
