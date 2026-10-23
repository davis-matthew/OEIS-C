/*
 * A123885  a(1)=1. a(n) = a(n-1) + (number of earlier terms {i.e., terms a(1)
 *          through a(n-1)} that divide n).
 * https://oeis.org/A123885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + (number of earlier terms that divide n)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses memoisation; computes terms only once and works for any n ≥ 1 */
#include <stdlib.h>

static seq_t *seq_table = NULL;  /* 1‑based array of a(1)…a(N) */
static idx_t  seq_allocated = 0; /* number of slots currently allocated */
static idx_t  seq_computed  = 0; /* highest index already computed */

/* Ensure the table has at least size slots (size ≥ n+1 for 1‑based indexing) */
static void ensure_allocated(idx_t size)
{
    if (seq_allocated < size) {
        idx_t new_alloc = seq_allocated ? seq_allocated << 1 : 8;
        if (new_alloc < size) new_alloc = size;
        seq_table = (seq_t *)realloc(seq_table, (size_t)new_alloc * sizeof(seq_t));
        seq_allocated = new_alloc;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined, but safe */

    ensure_allocated(n + 1);

    if (seq_computed < n) {
        /* Initialise first term if needed */
        if (seq_computed == 0) {
            seq_table[1] = (seq_t)1;
            seq_computed = 1;
        }

        /* Compute new terms up to n */
        for (idx_t i = seq_computed + 1; i <= n; ++i) {
            unsigned int cnt = 0;
            seq_t prev = seq_table[i - 1];
            /* Count earlier terms that divide i */
            for (idx_t k = 1; k < i; ++k) {
                seq_t ak = seq_table[k];
                if (ak > (seq_t)i) continue;           /* cannot divide */
                if (i % (idx_t)ak == 0) ++cnt;          /* safe cast: ak ≤ i ≤ 64‑bit */
            }
            seq_table[i] = prev + (seq_t)cnt;
        }
        seq_computed = n;
    }

    return seq_table[n];
}
