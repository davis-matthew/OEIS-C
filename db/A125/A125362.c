/*
 * A125362  Number of base 25 circular n-digit numbers with adjacent digits
 *          differing by 4 or less.
 * https://oeis.org/A125362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) where A[i][j] = 1 if |i-j| <= 4 (digits 0‑24),
 *            else 0. This counts the number of base‑25 circular n‑digit
 *            numbers whose adjacent digits differ by at most 4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

static bool seq_init_done = false;

/* Local data shared by all calls */
struct seq_work {
    seq_t power[25][25];      /* current A^k   */
    seq_t A[25][25];          /* adjacency     */
    seq_t *cache;             /* cached a(n)   */
    size_t cache_cap;         /* allocated size */
    idx_t cache_len;          /* number of terms stored, cache_len-1 is the highest n computed */
} seq_data;

/* Initialize the static data once */
static void seq_work_init(void) {
    if (seq_init_done) return;
    /* identity matrix for power (k = 0) */
    for (idx_t i = 0; i < 25; ++i)
        for (idx_t j = 0; j < 25; ++j)
            seq_data.power[i][j] = (i == j) ? 1 : 0;
    /* adjacency matrix A[i][j] = 1 if |i-j| <= 4 */
    for (idx_t i = 0; i < 25; ++i)
        for (idx_t j = 0; j < 25; ++j) {
            idx_t diff = (i > j) ? (i - j) : (j - i);
            seq_data.A[i][j] = (diff <= 4) ? 1 : 0;
        }
    /* allocate cache for a(0) */
    seq_data.cache_cap = 1;
    seq_data.cache = (seq_t*)malloc(seq_data.cache_cap * sizeof(seq_t));
    seq_data.cache[0] = 1;          /* a(0) = 1 */
    seq_data.cache_len = 1;         /* highest n computed = 0 */
    seq_init_done = true;
}

/* Multiply current power matrix by adjacency matrix A, store result back in power.
   Return trace(A^(k+1)). */
static seq_t seq_work_next_pow(void) {
    seq_t temp[25][25];
    /* temp = power * A */
    for (idx_t i = 0; i < 25; ++i)
        for (idx_t j = 0; j < 25; ++j) {
            seq_t sum = 0;
            for (idx_t k = 0; k < 25; ++k)
                sum += seq_data.power[i][k] * seq_data.A[k][j];
            temp[i][j] = sum;
        }
    /* Copy back to power */
    for (idx_t i = 0; i < 25; ++i)
        for (idx_t j = 0; j < 25; ++j)
            seq_data.power[i][j] = temp[i][j];
    /* Compute trace */
    seq_t trace = 0;
    for (idx_t i = 0; i < 25; ++i) trace += seq_data.power[i][i];
    return trace;
}

/* Ensure cache has room for index n, growing if necessary. */
static void seq_work_ensure_cache(idx_t n) {
    if (seq_data.cache_len > n) return;  /* already present */
    /* Grow cache to at least n+1 entries */
    size_t needed = (size_t)(n + 1);
    if (seq_data.cache_cap < needed) {
        size_t new_cap = seq_data.cache_cap ? seq_data.cache_cap * 2 : 1;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *new_cache = (seq_t*)realloc(seq_data.cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();  /* memory failure */
        seq_data.cache = new_cache;
        seq_data.cache_cap = new_cap;
    }
}

/* Main sequence function */
seq_t a(idx_t n) {
    seq_work_init();
    /* a(0) is already in cache[0] */
    seq_work_ensure_cache(n);
    while (seq_data.cache_len <= n) {
        /* compute next power and trace */
        seq_t trace = seq_work_next_pow();
        seq_data.cache[seq_data.cache_len] = trace;
        seq_data.cache_len++;
    }
    return seq_data.cache[n];
}
