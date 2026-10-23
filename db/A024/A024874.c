/*
 * A024874  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n-k+1), where k = [ n/2
 *          ], s = (natural numbers >= 2), t = (F(2), F(3), F(4), ...).
 * https://oeis.org/A024874
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊n/2⌋} (i+1) * F(n-i+2) where F(0)=0, F(1)=1
 *            (standard Fibonacci numbers)
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Offset is 2 – values for n < 2 are unspecified */
    if (n < 2)
        return (seq_t)0;

    /* Memoised Fibonacci numbers: fib_cache[i] = F(i) */
    static seq_t *fib_cache = NULL;
    static idx_t fib_size  = 0;   /* current cache size (next free index) */

    /* We may need Fibonacci numbers up to index n+1 (t(n)=F(n+1)). */
    idx_t max_index = n + 1;
    if (fib_size <= max_index) {
        /* Grow the cache */
        idx_t old_size = fib_size;
        idx_t new_size = max_index + 1;        /* indices 0 .. max_index */

        seq_t *new_ptr = NULL;
        if (fib_cache == NULL) {
            new_ptr = (seq_t *)malloc(new_size * sizeof(seq_t));
        } else {
            new_ptr = (seq_t *)realloc(fib_cache, new_size * sizeof(seq_t));
        }
        if (new_ptr == NULL)    /* allocation failed */
            abort();

        fib_cache = new_ptr;
        /* Initialise base Fibonacci values if this is the first allocation */
        if (old_size == 0) {
            fib_cache[0] = (seq_t)0;
            fib_cache[1] = (seq_t)1;
            old_size = 2;
        }

        /* Compute missing Fibonacci numbers */
        for (idx_t i = old_size; i <= max_index; ++i) {
            fib_cache[i] = fib_cache[i - 1] + fib_cache[i - 2];
        }
        fib_size = new_size;
    }

    /* Compute the sum */
    seq_t result = (seq_t)0;
    idx_t k = n / 2;   /* floor(n/2) */
    for (idx_t i = 1; i <= k; ++i) {
        seq_t s_val = (seq_t)(i + 1);           /* s(i) = i+1  (natural numbers >=2) */
        idx_t fib_idx = n - i + 2;              /* t(n-i+1) = F(n-i+2) */
        seq_t t_val = fib_cache[fib_idx];
        result += s_val * t_val;
    }
    return result;
}
