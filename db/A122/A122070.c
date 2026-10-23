/*
 * A122070  Triangle given by T(n,k) = Fibonacci(n+k+1)*binomial(n,k) for
 *          0<=k<=n.
 * https://oeis.org/A122070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Fib(r + k + 1) * binomial(r, k), where r = (sqrt(8*n + 1)
 *            - 1) / 2 (integer division) and k = n - r*(r + 1)/2
 * Range:     u8 n <= 7, u16 n <= 49, u32 n <= 161, u64 n <= 577,
 *            u128 n <= 2113, big any n
 * Helpers:   binomial, fib, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"
#include "../utils/fib.h"
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* Compute r = floor((sqrt(8*n + 1) - 1)/2) */
    seq_t val = (seq_t)8 * (seq_t)n + 1;          /* 8*n + 1 as seq_t */
    seq_t s   = floor_sqrt(val);                  /* floor(sqrt(8*n + 1)) */
    idx_t r   = (idx_t)((s - (seq_t)1) / (seq_t)2);

    /* Compute k = n - offset(row r) */
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;

    /* Fib(r + k + 1) */
    idx_t fib_n = r + k + 1;
    seq_t f = fib(fib_n);

    /* binomial(r, k) */
    seq_t b = binomial(r, k);

    return f * b;
}
