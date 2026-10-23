/*
 * A199192  Primes of the form 3^(2n-3)+3^(n-1)+1.
 * https://oeis.org/A199192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 5, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   is_prime_big, mul_small, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A199192: primes of the form 3^(2k‑3)+3^(k‑1)+1.
   a(n) returns the n‑th prime of this form, with offset 1.  The
   function repeatedly generates the candidate value and tests it
   for primality. */
#include <stdbool.h>
#include "../utils/pow3.h"
#include "../utils/mul_small.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset 1 – this case is not used */

    /* start with k = 2 (the first value that yields 7) */
    idx_t k = 2;
    seq_t pow_odd  = pow3(1);       /* 3^(k‑1)  for k=2 -> 3^1 */
    seq_t pow_even = pow3(1);       /* 3^(2k‑3) for k=2 -> 3^1 */
    idx_t count = 0;

    while (1)
    {
        /* candidate value = 3^(2k‑3) + 3^(k‑1) + 1 */
        seq_t val = pow_even + pow_odd + 1;

        if (is_prime_big(val))
        {
            ++count;
            if (count == n)
                return val;
        }

        /* move to the next k: multiply exponents by 3 and 9 */
        ++k;
        pow_odd  = mul_small(pow_odd, 3);   /* 3^(k‑1) = previous * 3 */
        pow_even = mul_small(pow_even, 9);  /* 3^(2k‑3) = previous * 9 */
    }
}
