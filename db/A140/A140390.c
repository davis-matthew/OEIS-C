/*
 * A140390  Nonprimes of the form 21k+1.
 * https://oeis.org/A140390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = nth positive integer ≡ 1 (mod 21) that is not prime. */
/* Computes the terms by scanning the progression 1, 22, 43, … and
   collecting the non‑prime members. */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Return the n‑th term (1‑based).  Calls with n < 1 return 0. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* defined only for n ≥ 1 */

    seq_t cand   = 1;                     /* 1 ≡ 1 (mod 21) */
    idx_t count  = 0;                     /* number of non‑primes seen */

    for (;;)
    {
        if (!is_prime_big(cand))          /* 1 counts as non‑prime */
        {
            ++count;
            if (count == n)
                return cand;
        }
        cand += 21;                       /* next candidate ≡ 1 (mod 21) */
    }
}
