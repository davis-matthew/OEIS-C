/*
 * A217762  Square array T, read by antidiagonals: T(n,k) = F(n) + 2*F(k)
 *          where F(n) is the n-th Fibonacci number.
 * https://oeis.org/A217762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A217762: square array T with T(n,k) = F(n) + 2*F(k), read by antidiagonals.
 * a(n) enumerates the entries of T by increasing antidiagonal sums.
 * The implementation uses an iterative search for the diagonal and a
 * lazily‑grown cache of Fibonacci numbers.
 */

#include <stddef.h>
#include <stdlib.h>

/* ------------------------------------------------------------ */
/* Fibonacci cache                                               */
/* ------------------------------------------------------------ */
static seq_t *fib_cache = NULL;   /* dynamically grown array of Fibonacci numbers   */
static size_t fib_cap   = 0;      /* current allocated capacity                     */
static idx_t  fib_last  = 0;      /* index of the last computed Fibonacci number   */

/* Ensure fib_cache contains Fibonacci numbers up to index k (inclusive). */
static void
ensure_fib(idx_t k)
{
    /* Initialise cache if it is empty. */
    if (fib_cap == 0) {
        fib_cap = 2;                     /* store F(0) and F(1)                     */
        fib_cache = (seq_t *)malloc(fib_cap * sizeof(seq_t));
        if (!fib_cache) exit(1);          /* abort on allocation failure             */
        fib_cache[0] = (seq_t)0;
        fib_cache[1] = (seq_t)1;
        fib_last  = 1;
    }

    /* Grow the array if necessary and compute missing Fibonacci values. */
    while (fib_last < k) {
        /* Grow capacity when we run out of space. */
        if ((size_t)(fib_last + 1) == fib_cap) {
            size_t new_cap = fib_cap * 2;
            seq_t *tmp = (seq_t *)realloc(fib_cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            fib_cache = tmp;
            fib_cap   = new_cap;
        }
        /* Compute the next Fibonacci number. */
        ++fib_last;
        fib_cache[fib_last] = fib_cache[fib_last - 1] + fib_cache[fib_last - 2];
    }
}

/* ------------------------------------------------------------ */
/* Sequence a(n)                                                */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Determine the diagonal d that contains the index n. */
    idx_t d   = 0;        /* current diagonal (number of rows in previous diagonals) */
    idx_t cum = 0;        /* total number of elements before the current diagonal  */

    while (cum + d + 1 <= n) {
        cum += d + 1;
        ++d;
    }

    /* Off‑diagonal position within diagonal d. */
    idx_t offset = n - cum;          /* 0‑based index in this diagonal                */
    idx_t i = offset;                /* row index                                    */
    idx_t j = d - offset;            /* column index                                 */

    /* Compute needed Fibonacci numbers. */
    idx_t need = (i > j ? i : j);
    ensure_fib(need);

    seq_t Fi = fib_cache[i];
    seq_t Fj = fib_cache[j];

    /* Return T(i,j) = F(i) + 2*F(j). */
    return Fi + (seq_t)2 * Fj;
}
