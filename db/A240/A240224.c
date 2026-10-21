/*
 * A240224  Irregular triangular array read by rows: row n gives a list of the
 *          partitions of n into distinct Fibonacci numbers. The order of the
 *          partitions is like in Abramowitz-Stegun.
 * https://oeis.org/A240224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) enumerates the parts of the partitions of integers into
 *            distinct Fibonacci numbers, read row‑wise, each partition's
 *            parts listed in descending order and partitions sorted by
 *            increasing number of parts and lexicographically
 * Range:     u8 n <= 108, u16 n <= 108, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Helpers:   append_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/append_candidate.h" /* provided helper */

static seq_t *term_cache = NULL;      /* flattened array of parts           */
static size_t term_cache_len = 0;     /* number of cached terms             */
static size_t term_cache_cap = 0;     /* capacity of term_cache             */
static idx_t rows_computed = 0;       /* largest row fully computed         */

/* Fibonacci numbers (distinct Fibonacci numbers starting 1,2) */
static seq_t *fib_arr = NULL;
static size_t fib_len = 0;            /* number of fib numbers <= current limit */
static size_t fib_cap = 0;            /* capacity of fib_arr                  */

/* ensure fib_arr contains all Fibonacci numbers <= limit */
static void ensure_fib_upto(idx_t limit)
{
    if (fib_len == 0) {
        fib_cap = 4;
        fib_arr = (seq_t *)malloc(fib_cap * sizeof(seq_t));
        fib_arr[0] = 1;
        fib_arr[1] = 2;
        fib_len = 2;
    }
    while (fib_len > 0 && fib_arr[fib_len - 1] <= limit) {
        if (fib_cap <= fib_len) {
            fib_cap = fib_cap ? fib_cap * 2 : 4;
            fib_arr = (seq_t *)realloc(fib_arr, fib_cap * sizeof(seq_t));
        }
        seq_t next = fib_arr[fib_len - 1] + fib_arr[fib_len - 2];
        fib_arr[fib_len++] = next;
    }
}

/* recursive generation of partitions of 'remaining' into 'k' distinct
   Fibonacci numbers, choosing indices <= max_idx */
static void gen_k(seq_t remaining, int max_idx, int k, int depth,
                  seq_t *parts,
                  seq_t **p_cache, size_t *len, size_t *cap)
{
    if (k == 0) {
        if (remaining == 0) {
            for (int i = 0; i < depth; ++i)
                append_candidate(p_cache, len, cap, parts[i]);
        }
        return;
    }
    for (int i = max_idx; i >= 0; --i) {
        seq_t val = fib_arr[i];
        if (val > remaining) continue;
        parts[depth] = val;
        gen_k(remaining - val, i - 1, k - 1, depth + 1,
              parts, p_cache, len, cap);
    }
}

/* generate all partitions of row into distinct Fibonacci numbers
   and append their parts to the global cache */
static void generate_row(idx_t row)
{
    ensure_fib_upto(row);
    int max_idx = (int)fib_len - 1;
    seq_t parts[100]; /* sufficient for all fib counts < 100 */

    /* partitions with increasing number of parts */
    for (int k = 1; k <= max_idx + 1; ++k)
        gen_k((seq_t)row, max_idx, k, 0, parts,
              &term_cache, &term_cache_len, &term_cache_cap);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, but safe */
    if (n <= term_cache_len)       /* already cached */
        return term_cache[n - 1];

    /* compute rows until we have enough terms */
    for (idx_t r = rows_computed + 1; term_cache_len < n; ++r) {
        generate_row(r);
        rows_computed = r;
    }
    return term_cache[n - 1];
}
