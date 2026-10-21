/*
 * A050291  Number of double-free subsets of {1, 2, ..., n}.
 * https://oeis.org/A050291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = ∏_{m odd ≤ n} Fib(⌊log₂(n/m)⌋ + 3)
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 38, u64 n <= 77, u128 n <= 155,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where Fib(1)=1, Fib(2)=1, Fib(k)=Fib(k-1)+Fib(k-2) */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Pre‑compute Fibonacci numbers up to the maximum needed index. */
    static seq_t fib[21];          /* enough for n ≤ 10 000 (index ≤ 16) */
    static int fib_init = 0;
    if (!fib_init) {
        fib[0] = 0;
        fib[1] = 1;
        for (size_t i = 2; i < 21; ++i) {
            fib[i] = fib[i-1] + fib[i-2];
        }
        fib_init = 1;
    }

    seq_t prod = 1;
    for (idx_t m = 1; m <= n; m += 2) {   /* iterate over odd m */
        idx_t t = m;
        idx_t L = 1;                      /* length of the chain starting at m */
        while (t <= n/2) {                /* while 2*t ≤ n */
            t <<= 1;                      /* t = 2*t */
            ++L;
        }
        /* Fibonacci index needed is L + 2 */
        prod *= fib[L + 2];
    }

    return prod;
}
