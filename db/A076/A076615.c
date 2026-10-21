/*
 * A076615  Number of permutations of {1,2,...,n} that result in a binary
 *          search tree with the minimum possible height.
 * https://oeis.org/A076615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 15, u64 n <= 23, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula for A076615:
   a(n) = b(n, floor(log2(n)) + 1)
   where b(n,k) satisfies
   b(0,k) = 1
   b(1,0) = 0,  b(1,k>0) = 1
   b(n,0) = 0 (n≥2)
   b(n,k) = Σ_{r=1}^{n} C(n-1,r-1) · b(r-1,k-1) · b(n-r,k-1)   (n≥2, k≥1) 
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* external types provided by the build */
extern seq_t a(idx_t n);

/* ---------- internal state ---------- */
static seq_t *table   = NULL;     /* flattened 2‑D table: index = n*(max_k+1)+k   */
static idx_t  max_n   = 0;        /* largest n already computed */
static int    max_k   = 0;        /* largest k already computed */

/* floor(log2(v)) for v>0 */
static inline int floor_log2_idx(idx_t v)
{
    /* v is always >0 when called */
    return 63 - __builtin_clzll(v);
}

/* Reallocate and recompute the whole table for new_n and new_k */
static void ensure_size(idx_t new_n, int new_k)
{
    if (new_n <= max_n && new_k <= max_k) return;

    /* allocate fresh table */
    size_t sz = (size_t)(new_n + 1) * (new_k + 1);
    seq_t *new_table = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!new_table) return;           /* allocation failure – caller will fail */

    /* clear */
    for (size_t i = 0; i < sz; ++i) new_table[i] = (seq_t)0;

    /* free old table if any */
    if (table) free(table);

    table = new_table;
    max_n = new_n;
    max_k = new_k;

    /* compute all entries */
    for (idx_t n = 0; n <= max_n; ++n) {
        for (int k = 0; k <= max_k; ++k) {
            seq_t *cell = &table[n * (max_k + 1) + k];

            if (n == 0) {                    /* b(0,k)=1 */
                *cell = (seq_t)1;
            } else if (n == 1) {             /* b(1,0)=0, b(1,k>0)=1 */
                *cell = (k == 0) ? (seq_t)0 : (seq_t)1;
            } else if (k == 0) {             /* n≥2, depth 0 impossible */
                *cell = (seq_t)0;
            } else {                         /* recurrence */
                seq_t sum = (seq_t)0;
                seq_t bin = (seq_t)1;        /* C(n-1,0) */

                for (idx_t i = 0; i < n; ++i) {
                    seq_t left  = table[i * (max_k + 1) + (k - 1)];
                    seq_t right = table[(n - 1 - i) * (max_k + 1) + (k - 1)];
                    seq_t prod  = left * right;
                    prod = prod * bin;
                    sum += prod;

                    /* update bin to C(n-1,i+1) for next i, if needed */
                    if (i + 1 < n) {
                        bin = bin * (seq_t)(n - 1 - i);
                        bin = bin / (seq_t)(i + 1);
                    }
                }
                *cell = sum;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;            /* a(0) = 1 by definition */

    int k_needed = floor_log2_idx(n) + 1;
    ensure_size(n, k_needed);
    return table[n * (max_k + 1) + k_needed];
}
