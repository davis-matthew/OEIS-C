/*
 * A192974  Coefficient of x in the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A192974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+7) + Lucas(n+3) - 2*n*(n+4) - 17 Recurrence:
 *            a(n) = 4*a(n-1) - 5*a(n-2) + a(n-3) + 2*a(n-4) - a(n-5), with
 *            a(0)=0, a(1)=1, a(2)=4, a(3)=14, a(4)=37. The implementation
 *            below evaluates the closed‑form expression using a threaded
 *            cache of Fibonacci numbers; Lucas(n) = F(n-1)+F(n+1)
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 40, u64 n <= 86, u128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of Fibonacci numbers up to the largest index needed so far. */
    static seq_t *fib = NULL;
    static idx_t fib_len = 0;          /* current allocated length */

    /* We require Fibonacci numbers up to index n+7. */
    idx_t required = n + 8;            /* indices 0 .. n+7 inclusive */

    if (required > fib_len) {
        /* Allocate (or grow) the array.  The cast is required for C++ */
        seq_t *newfib = (seq_t *)realloc(fib, required * sizeof(seq_t));
        if (newfib == NULL) abort();
        fib = newfib;

        /* Initialise the first two Fibonacci numbers on first use. */
        if (fib_len == 0) {
            fib[0] = 0;
            fib[1] = 1;
            fib_len = 2; /* we now have indices 0 and 1 ready */
        }

        /* Compute any new Fibonacci terms that are needed. */
        for (idx_t i = fib_len; i < required; ++i)
            fib[i] = fib[i - 1] + fib[i - 2];

        fib_len = required;
    }

    /* Compute the closed‑form formula. */
    seq_t term = (seq_t)2 * ((seq_t)n * ((seq_t)n + (seq_t)4)) + (seq_t)17;

    seq_t lucas = fib[n + 2] + fib[n + 4];   /* Lucas(n+3) */
    return fib[n + 7] + lucas - term;
}
