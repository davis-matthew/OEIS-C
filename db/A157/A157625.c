/*
 * A157625  Product of the composite numbers between n+1 and 2n, both
 *          inclusive.
 * https://oeis.org/A157625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of composite integers m with n+1 ≤ m ≤ 2n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   is_prime, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function relies on the utilities:
   - is_prime(idx_t n)  : returns true if n is a prime.
   - seq_mul(seq_t a, seq_t b) : multiplies two seq_t values. */

/* Include required headers. */
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/seq_mul.h"

/* Compute the value of sequence A157625 at index n. */
seq_t a(idx_t n)
{
    /* For n < 1 the sequence is undefined; return 0. */
    if (n < 1) return 0;

    /* Initialize product to 1. This works for all seq_t
       types (built‑in integers and arbitrary‑precision). */
    seq_t prod = 1;

    /* Iterate over the interval [n+1, 2n] and multiply if composite. */
    for (idx_t m = n + 1; m <= 2 * n; ++m)
    {
        if (!is_prime(m))
            prod = seq_mul(prod, (seq_t)m);
    }

    return prod;
}
