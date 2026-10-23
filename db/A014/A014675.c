/*
 * A014675  The infinite Fibonacci word (start with 1, apply 1->2, 2->21, take
 *          limit).
 * https://oeis.org/A014675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = floor((n+2)*phi) - floor((n+1)*phi)
 * Range:     u8 n <= 1554, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Precompute Fibonacci numbers (F(0)=0, F(1)=1).  The infinite
   Fibonacci word uses lengths F(n+1).  We keep enough entries for
   n up to 10 000 or much larger.  The array is enlarged lazily. */
static seq_t fib[200] = {0};
static size_t fib_n = 0;   /* number of fib entries currently stored (>=2) */

/* Return the nth term of A014675 (0‑based).  The function works for any
   idx_t n, including large values, as long as the required Fibonacci
   number fits into seq_t or the array limit is not exceeded. */
seq_t a(idx_t n) {
    /* Initialise Fibonacci table once. */
    if (fib_n == 0) {        /* not yet initialised */
        fib[0] = (seq_t)0;
        fib[1] = (seq_t)1;
        fib_n = 2;           /* we have F(0) and F(1) */
    }

    /* Ensure we have a Fibonacci value larger than n. */
    while (fib_n < sizeof(fib)/sizeof(fib[0]) &&
           fib[fib_n-1] <= (seq_t)n) {
        fib[fib_n] = fib[fib_n-1] + fib[fib_n-2];
        fib_n++;
    }

    /* Find the smallest k >= 1 with fib[k+1] > n.  Such k exists
       because fib[k+1] grows unbounded. */
    size_t k = 1;
    while (k+1 < fib_n && fib[k+1] <= (seq_t)n)
        k++;

    /* Iteratively walk down the recursion tree:  S(k) = S(k-1) S(k-2). */
    idx_t pos = n;
    while (1) {
        if (k == 0) return (seq_t)1;   /* S(0) = "1" */
        if (k == 1) return (seq_t)2;   /* S(1) = "2" */
        if ((seq_t)pos < fib[k]) {
            /* We are in the first part S(k-1). */
            k--;
        } else {
            /* We are in the second part S(k-2); adjust the index. */
            pos -= (idx_t)fib[k];
            k -= 2;
        }
    }
}
