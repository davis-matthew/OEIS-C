/*
 * A100930  Semiprimes of the form (2*p+1)*(p-1)/2, p prime.
 * https://oeis.org/A100930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*A059455(n)^2 - A059455(n) - 1) / 2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------ */
/*  A100930 – Semiprimes of the form (2*p+1)*(p-1)/2, p prime. */
/* ------------------------------------------------------------ */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_uint64.h"

/* ----------------------------------------------------------------
   The sequence is defined by the primes p satisfying:
      p                   prime
      q = (p-1)/2         prime
      r = 2p+1            prime
   For such a p the term is
        a = (2p+1)*(p-1)/2
   We simply test odd numbers p for the three primality conditions
   using the deterministic 64‑bit Miller–Rabin routine provided by
   is_prime_uint64.  The search stops as soon as the requested index
   has been reached.  This method runs well below the time limit
   for n up to about 10 000.
   ---------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* n≥1 because OEIS offset 1.  For safety, handle n==0. */
    if (n == 0) return 0;

    idx_t found = 0;
    for (uint64_t p = 5; ; p += 2)            /* only odd candidates */
    {
        if (!is_prime_uint64(p))
            continue;                        /* skip non‑prime p */

        uint64_t f1 = (p - 1) >> 1;           /* (p-1)/2   */
        if (!is_prime_uint64(f1))
            continue;                        /* q not prime */

        uint64_t f2 = (p << 1) + 1;           /* 2p+1      */
        if (!is_prime_uint64(f2))
            continue;                        /* r not prime */

        ++found;
        if (found == n)                      /* desired term found */
        {
            /* a = (2p+1)*(p-1)/2 = f2*(p-1)/2  */
            seq_t val = (seq_t)f2 * (seq_t)(p - 1) / 2;
            return val;
        }
    }
}
