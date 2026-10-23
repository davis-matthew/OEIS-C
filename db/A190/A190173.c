/*
 * A190173  a(n) = Sum_{1 <= i < j <= n} F(i)*F(j), where F(k) is the k-th
 *          Fibonacci number.
 * https://oeis.org/A190173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(n+1)^2 - F(n+2) + (1-(-1)^n)/2 where F(k) is the k‑th
 *            Fibonacci number (F(0)=0, F(1)=1)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*--- cache of Fibonacci numbers ------------------------------------------------*/
/* fib_cache[k] = standard Fibonacci F(k) (F(0)=0, F(1)=1).  The array is
   grown on demand and kept across calls so that the sequence can be
   evaluated in any order.  */
static seq_t *fib_cache = NULL;      /* pointer to first element (index 0)   */
static size_t fib_cache_size = 0;    /* number of elements allocated        */

/* Ensure that fib_cache is large enough to contain indices 0..max_index.    */
static void
ensure_fib(idx_t max_index)
{
    if (fib_cache_size <= (size_t)max_index) {
        size_t new_size = (size_t)max_index + 1;          /* indices 0..max_index */
        seq_t *new_cache = (seq_t *)realloc(fib_cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();                         /* allocation failed */

        /* Initialise new portion of the cache.  For the first call we
           must also set F(0)=0 and F(1)=1.  */
        if (fib_cache == NULL) {
            new_cache[0] = (seq_t)0;
            if (new_size > 1) new_cache[1] = (seq_t)1;
        }

        /* Compute Fibonacci numbers that were not present before.  */
        for (idx_t i = fib_cache_size; i <= (idx_t)max_index; ++i) {
            if (i < 2) continue;                         /* F(0) and F(1) already set */
            new_cache[i] = new_cache[i-1] + new_cache[i-2];
        }

        fib_cache = new_cache;
        fib_cache_size = new_size;
    }
}

/*--- sequence a(n) -----------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n < 1, but return 0 */

    /* We need Fibonacci numbers up to index n+2 (standard indexing).   */
    ensure_fib(n + 2);

    seq_t fnp1 = fib_cache[n + 1];   /* F(n+1) */
    seq_t fnp2 = fib_cache[n + 2];   /* F(n+2) */

    seq_t parity = (seq_t)(n & 1U);  /* (1-(-1)^n)/2 gives 1 if n odd, 0 if even */

    return fnp1 * fnp1 - fnp2 + parity;
}
