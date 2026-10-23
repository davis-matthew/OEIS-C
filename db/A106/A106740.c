/*
 * A106740  Triangle read by rows: greatest common divisors of pairs of
 *          Fibonacci numbers greater than 1: T(n, k) = gcd(Fibonacci(n),
 *          Fibonacci(k)).
 * https://oeis.org/A106740
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Fibonacci(gcd(r, c)), where n is the flattened index
 *            (offset 3), r = row (≥ 3), c = column (≥ 3 ≤ r) of the triangle
 *            T(r, c) = gcd(Fibonacci(r), Fibonacci(c)) = Fibonacci(gcd(r, c))
 * Range:     u8 n <= 79, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   fib_fast
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib_fast.h"

/* Euclidean algorithm for 64‑bit unsigned integers. */
static inline uint64_t
gcd64(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Return Fibonacci(n) using the fast‑doubling routine from utils. */
static inline seq_t
fib_of(idx_t n)
{
    seq_t fn, fn1;
    fib_fast(n, &fn, &fn1);   /* fn = F(n), fn1 = F(n+1) */
    return fn;
}

/* Sequence function */
seq_t
a(idx_t n)
{
    if (n < 3) return (seq_t)0;          /* undefined below the offset */

    /* Convert to zero‑based index in the flattened list. */
    idx_t p = n - 3;

    /* Find the row r such that the cumulative element count up to row r
       exceeds p.  The number of elements in row r is r‑2. */
    idx_t r = 3;
    for (;;) {
        idx_t cum = (r - 2) * (r - 1) / 2;   /* total elements up to row r */
        if (cum > p) break;
        ++r;
    }

    /* Number of elements in preceding rows: (r‑3)(r‑2)/2. */
    idx_t prev = (r - 3) * (r - 2) / 2;
    idx_t offset = p - prev;                /* 0‑based position inside row r */
    idx_t c = 3 + offset;                   /* column index */

    /* GCD of the row and column indices. */
    uint64_t g = gcd64((uint64_t)r, (uint64_t)c);

    /* Return the Fibonacci number at index g. */
    return fib_of(g);
}
