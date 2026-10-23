/*
 * A108046  Inverse Moebius transform of Fibonacci numbers 0, 1, 1, 2, 3, 5,
 *          8, ...
 * https://oeis.org/A108046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} Fibonacci(d-1) where Fibonacci(0)=0,
 *            Fibonacci(1)=1
 * Range:     u8 n <= 14, u16 n <= 25, u32 n <= 48, u64 n <= 94, u128 n <= 187,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *fib = NULL;          /* array of Fibonacci numbers */
static idx_t fib_len = 0;          /* number of Fibonacci numbers stored */
static idx_t fib_cap = 0;          /* capacity of the fib array */

/* Ensure that fib[0..n] are computed. n is a 0‑based index for Fibonacci. */
static void
ensure_fib(idx_t n)
{
    if (n < fib_len)
        return;                   /* already computed */
    if (fib == NULL) {            /* first time, initialise */
        fib_cap = 16;
        fib = (seq_t *)malloc(sizeof(seq_t) * fib_cap);
        fib[0] = (seq_t)0;
        fib[1] = (seq_t)1;
        fib_len = 2;
    }
    while (fib_len <= n) {
        if (fib_len == fib_cap) {   /* grow array */
            fib_cap <<= 1;
            fib = (seq_t *)realloc(fib, sizeof(seq_t) * fib_cap);
        }
        fib[fib_len] = fib[fib_len - 1] + fib[fib_len - 2];
        ++fib_len;
    }
}

seq_t a(idx_t n)
{
    /* n >= 1 by specification */
    idx_t i;
    seq_t sum = (seq_t)0;

    /* Make sure Fibonacci numbers up to n-1 exist. */
    if (n == 0)
        return sum;   /* safety: though n should not be 0 per spec */
    ensure_fib(n - 1);

    /* Enumerate divisors of n */
    for (i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            seq_t f1 = fib[i - 1];
            sum += f1;
            idx_t d2 = n / i;
            if (d2 != i) {
                seq_t f2 = fib[d2 - 1];
                sum += f2;
            }
        }
    }
    return sum;
}
