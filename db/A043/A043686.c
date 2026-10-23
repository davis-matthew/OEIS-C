/*
 * A043686  a(n) = (1/2)*(n-th number whose base-2 representation has exactly
 *          12 runs).
 * https://oeis.org/A043686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (1/2) * (n-th positive integer whose binary
 *            representation has exactly 12 runs)
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Compare two seq_t values – works for any seq_t type. */
static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return  1;
    return 0;
}

/* Cache of sequence values (a(n)). */
static seq_t *cache     = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static bool   sorted     = true;

/* Current bit length to generate numbers for (starting from 12). */
static idx_t current_bitlen = 12;

/* Append a new value into the cache. */
static void add_value(seq_t val)
{
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_size++] = val;
    sorted = false;
}

/* Recursively generate all run‑length compositions of 12 runs
   with the specified remaining extra bits.  */
static void gen_runs_recursive(int idx, int rem, unsigned int *len)
{
    if (idx == 12) {
        if (rem != 0) return;         /* only generate when all extras are used */
        /* Build the binary number N described by len[0..11]. */
        seq_t N = 0;
        for (int i = 0; i < 12; ++i) {
            unsigned int l = len[i];
            if ((i & 1) == 0) {            /* run of 1s */
                seq_t mask = ((seq_t)1 << l) - 1;
                N = (N << l) | mask;
            } else {                       /* run of 0s */
                N <<= l;
            }
        }
        /* N is even; a = N / 2. */
        add_value(N >> 1);
        return;
    }

    for (int e = 0; e <= rem; ++e) {
        len[idx] = 1 + e;                     /* run length = 1 + extra bits */
        gen_runs_recursive(idx + 1, rem - e, len);
    }
}

/* Generate all numbers with exactly 12 runs and bit-length L. */
static void generate_with_len(idx_t L)
{
    int rem = (int)(L - 12);
    unsigned int len[12];
    gen_runs_recursive(0, rem, len);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* below offset – undefined */

    /* Produce values until we have at least n entries. */
    while (cache_size < (size_t)n) {
        generate_with_len(current_bitlen++);
    }

    /* Sort the cache once it has new entries. */
    if (!sorted) {
        qsort(cache, cache_size, sizeof(seq_t), cmp_seq);
        sorted = true;
    }

    return cache[(size_t)n - 1];
}
