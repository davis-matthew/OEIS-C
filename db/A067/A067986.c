/*
 * A067986  Convolution of L(n+1) := A000204(n+1) (Lucas), n>=0, with L(n+8),
 *          n>=0.
 * https://oeis.org/A067986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (76*n+47)*F(n+1) + 47*(n+1)*F(n) where F(k) is the
 *            Fibonacci sequence with F(0)=0, F(1)=1
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoised array of Fibonacci numbers */
static seq_t *fib = NULL;
static idx_t fib_cap = 0;

/* Ensure fib[0..up_to] is computed and stored */
static void ensure_fib(idx_t up_to)
{
    if (up_to < fib_cap)
        return;

    /* Allocate or resize */
    seq_t *tmp = (seq_t *)realloc(fib, (up_to + 1) * sizeof(seq_t));
    if (!tmp) {
        /* In a real program we would handle the error; here we abort */
        abort();
    }
    fib = tmp;

    /* Compute new entries */
    for (idx_t i = fib_cap; i <= up_to; ++i) {
        if (i == 0) {
            fib[0] = 0;
        } else if (i == 1) {
            fib[1] = 1;
        } else {
            fib[i] = fib[i - 1] + fib[i - 2];
        }
    }
    fib_cap = up_to + 1;
}

seq_t a(idx_t n)
{
    /* We need Fibonacci numbers up to F(n+1) */
    ensure_fib(n + 1);

    /* Compute coefficients as seq_t to avoid overflow on the indices */
    seq_t coeff1 = ((seq_t)76 * (seq_t)n) + (seq_t)47;
    seq_t coeff2 = ((seq_t)47 * ((seq_t)n + (seq_t)1));

    /* Final result */
    return coeff1 * fib[n + 1] + coeff2 * fib[n];
}
