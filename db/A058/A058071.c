/*
 * A058071  A Fibonacci triangle: triangle T(n,k) =
 *          Fibonacci(k+1)*Fibonacci(n-k+1), for n >= 0, 0 <= k <= n.
 * https://oeis.org/A058071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl, nice
 * Formula:   a(n) = Fibonacci(k+1) * Fibonacci(r-k+1) where r =
 *            floor((sqrt(8*n+1)-1)/2) (row index, 0‑based) k = n - r*(r+1)/2
 *            (column index within that row) Entries of the Fibonacci triangle
 *            T(n,k) = Fib(k+1)*Fib(n-k+1) are read row by row to form the
 *            sequence
 * Range:     u8 n <= 90, u16 n <= 299, u32 n <= 1127, u64 n <= 4370,
 *            u128 n <= 7380, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Maximum n expected is about 10000; the algorithm runs in O(sqrt(n))
   per call and memoises Fibonacci numbers.  */

#include <stdlib.h>

/* static storage for Fibonacci numbers */
static seq_t *fib = NULL;   /* fib[i] = Fib(i)  with fib[0] = 0, fib[1] = 1 */
static idx_t fib_cap = 0;   /* highest index that has been computed (>=1) */

/* ensure that fib[0 .. need] are available */
static void ensure_fib(idx_t need)
{
    if (need <= fib_cap) return;

    idx_t new_cap = fib_cap ? fib_cap : 1;
    while (new_cap < need)
        new_cap <<= 1;

    seq_t *new_arr = (seq_t *)realloc(fib, (new_cap + 1) * sizeof(seq_t));
    if (!new_arr) abort();
    fib = new_arr;

    if (fib_cap == 0) {   /* first time */
        fib[0] = (seq_t)0;
        fib[1] = (seq_t)1;
        fib_cap = 1;
    }

    for (idx_t i = fib_cap + 1; i <= new_cap; ++i)
        fib[i] = fib[i - 1] + fib[i - 2];

    fib_cap = new_cap;
}

/* find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
static idx_t find_row(idx_t n)
{
    idx_t r = 0;
    while (1) {
        unsigned __int128 t = (unsigned __int128)(r + 1) * (r + 2) / 2;
        if (t > n) break;
        ++r;
    }
    return r;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* locate row r and position k within that row */
    idx_t r = find_row(n);
    unsigned __int128 start = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = (idx_t)(n - start);

    /* make sure Fibonacci numbers up to r+1 are available */
    ensure_fib(r + 1);

    /* compute T(r,k) = Fib(k+1) * Fib(r-k+1) */
    seq_t v1 = fib[k + 1];
    seq_t v2 = fib[r - k + 1];
    return v1 * v2;
}
