/*
 * A059727  a(n) = Fibonacci(n)*(Fibonacci(n) + 1).
 * https://oeis.org/A059727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n) * ( Fibonacci(n) + 1 )
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes the n-th Fibonacci number using the fast‑doubling method.
 * The algorithm runs in O(log n) time and uses only integer
 * arithmetic.  It is well suited for all supported seq_t types
 * (built‑in unsigned, unsigned __int128 and arbitrary‑precision C++ types).
 */
#include <stdint.h>   /* for idx_t definitions */

static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }

    seq_t a, b;
    fib_pair(n >> 1, &a, &b);          /* a = F_k, b = F_{k+1} */

    /* c = F_{2k}   = F_k * (2*F_{k+1} - F_k) */
    seq_t two_b = b + b;
    seq_t c = a * (two_b - a);

    /* d = F_{2k+1} = F_k^2 + F_{k+1}^2 */
    seq_t d = a * a + b * b;

    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = c + d;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);
    return fn * (fn + (seq_t)1);
}
