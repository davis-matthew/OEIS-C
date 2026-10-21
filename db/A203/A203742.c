/*
 * A203742  Vandermonde sequence using x^2 + xy + y^2 applied to the first n
 *          Fibonacci numbers, A000045.
 * https://oeis.org/A203742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{1 ≤ i < j ≤ n} (F_i^2 + F_i*F_j + F_j^2) where F_k is
 *            the k‑th Fibonacci number (starting with F_1 = 1)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/fib.h"

/* Grow an array to at least 'needed' elements */
static void grow_array(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap)
        return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed)
        new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp)
        abort();  /* memory exhausted */
    *ptr = tmp;
    *cap = new_cap;
}

/* Cache for the sequence values and Fibonacci numbers */
static seq_t *seq_cache = NULL;
static size_t seq_cap = 0;
static seq_t *fib_cache = NULL;
static size_t fib_cap = 0;
static idx_t current_max = 0;

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Ensure the cache can hold index n */
    grow_array(n + 1, &seq_cap, (void **)&seq_cache, sizeof(seq_t));
    grow_array(n + 1, &fib_cap, (void **)&fib_cache, sizeof(seq_t));

    /* Initialisation for n = 1 */
    if (current_max == 0) {
        fib_cache[1] = fib(1);
        seq_cache[1] = (seq_t)1;
        current_max = 1;
    }

    /* Extend precomputed range up to n */
    if (n > current_max) {
        /* Compute new Fibonacci numbers */
        for (idx_t i = current_max + 1; i <= n; ++i)
            fib_cache[i] = fib(i);

        /* Compute sequence values incrementally */
        for (idx_t m = current_max + 1; m <= n; ++m) {
            seq_t prod = seq_cache[m - 1];
            seq_t fm   = fib_cache[m];

            /* Multiply by each new factor (F_i^2 + F_i*F_m + F_m^2) for i < m */
            for (idx_t i = 1; i < m; ++i) {
                seq_t fi = fib_cache[i];
                /* (fi*fi + fi*fm + fm*fm) */
                seq_t val = fi * fi + fi * fm + fm * fm;
                prod *= val;
            }
            seq_cache[m] = prod;
        }
        current_max = n;
    }

    return seq_cache[n];
}
