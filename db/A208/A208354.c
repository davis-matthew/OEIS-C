/*
 * A208354  Number of compositions of n with at most one even part.
 * https://oeis.org/A208354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = T(n+1) – T(n-1) where T(0)=0 and
 *            T(n)=((2*n+3)*Fib(n)-n*Fib(n-1))/5 Fib(0)=0, Fib(1)=1,
 *            Fib(k)=Fib(k-1)+Fib(k-2)
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 41, u64 n <= 87, u128 n <= 176,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* cached Fibonacci array */
static seq_t *fib = NULL;
static size_t fib_cap = 0;
static size_t fib_len = 0;

/* ensure Fibonacci numbers up to index N are available */
static void ensure_fib(idx_t N)
{
    /* initialise if first use */
    if (fib == NULL) {
        fib_cap = 2;
        fib_len = 2;
        fib = (seq_t *)malloc(fib_cap * sizeof(seq_t));
        fib[0] = 0;
        fib[1] = 1;
    }
    /* grow capacity if needed */
    if ((size_t)(N + 1) > fib_cap) {
        size_t new_cap = fib_cap * 2;
        if (new_cap < (size_t)(N + 2))
            new_cap = (size_t)(N + 2);
        seq_t *new_fib = (seq_t *)malloc(new_cap * sizeof(seq_t));
        for (size_t i = 0; i < fib_len; ++i)
            new_fib[i] = fib[i];
        free(fib);
        fib = new_fib;
        fib_cap = new_cap;
    }
    /* compute missing Fibonacci entries */
    while (fib_len <= (size_t)N) {
        fib[fib_len] = fib[fib_len - 1] + fib[fib_len - 2];
        ++fib_len;
    }
}

/* T(n) for n >= 0, with T(0)=0 */
static seq_t T(idx_t n)
{
    if (n == 0) return 0;
    seq_t Fn     = fib[n];
    seq_t Fnm1   = fib[n - 1];
    seq_t tn    = (seq_t)(2 * n) + 3;      /* 2*n+3 as seq_t */
    seq_t part1 = tn * Fn;
    seq_t part2 = (seq_t)n * Fnm1;
    seq_t num   = part1 - part2;
    return num / 5;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* a(0)=1 by definition */
    ensure_fib(n + 1);             /* need Fib(n+1) and Fib(n-1) */
    seq_t t_plus  = T(n + 1);      /* T(n+1) */
    seq_t t_minus = T(n - 1);      /* T(n-1) */
    return t_plus - t_minus;
}
