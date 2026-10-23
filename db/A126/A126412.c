/*
 * A126412  Number of base 25 n-digit numbers with adjacent digits differing
 *          by two or less.
 * https://oeis.org/A126412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=25.
 * For n>=2 let dp_k[j] be the number of k‑digit base‑25 strings ending with digit j
 * (0≤j≤24). Then dp_1[j]=1 for all j and
 *   dp_{k+1}[j] = Σ_{i=max(0,j-2)}^{min(24,j+2)} dp_k[i].
 * The required count is
 *   a(n) = Σ_{j=0}^{24} dp_n[j].
 */
#include <stdlib.h>

static seq_t *a_cache = NULL;       /* memoised a(n) values */
static idx_t  a_cache_size = 0;     /* allocated size of a_cache */
static idx_t  max_n_computed = 0;   /* largest n for which we have a value */
static seq_t  dp_cur[25];           /* dp values for current length */

seq_t a(idx_t n)
{
    /* Initialisation on first call */
    if (max_n_computed == 0) {
        /* Allocate space for a(0) and a(1) */
        a_cache_size = 16;
        a_cache = (seq_t*)malloc(a_cache_size * sizeof(seq_t));
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)25;
        max_n_computed = 1;
        /* dp for length 1 : all 1's */
        for (int j = 0; j < 25; ++j) dp_cur[j] = (seq_t)1;
    }

    if (n <= max_n_computed) return a_cache[n];

    /* Ensure a_cache can hold up to n */
    if (n + 1 > a_cache_size) {
        size_t new_size = a_cache_size ? a_cache_size : 16;
        while (new_size < (size_t)(n + 1)) new_size <<= 1;
        seq_t *tmp = (seq_t*)realloc(a_cache, new_size * sizeof(seq_t));
        a_cache = tmp;
        a_cache_size = new_size;
    }

    /* Iterate from current max_n_computed up to n-1 */
    for (idx_t len = max_n_computed; len < n; ++len) {
        seq_t next[25];
        for (int j = 0; j < 25; ++j) {
            seq_t s = (seq_t)0;
            int i_start = j - 2;
            if (i_start < 0) i_start = 0;
            int i_end = j + 2;
            if (i_end > 24) i_end = 24;
            for (int i = i_start; i <= i_end; ++i) s += dp_cur[i];
            next[j] = s;
        }
        seq_t total = (seq_t)0;
        for (int j = 0; j < 25; ++j) total += next[j];
        a_cache[len + 1] = total;
        for (int j = 0; j < 25; ++j) dp_cur[j] = next[j];
    }

    max_n_computed = n;
    return a_cache[n];
}
