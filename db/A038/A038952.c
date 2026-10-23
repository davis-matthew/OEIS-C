/*
 * A038952  Primes p such that 69 is not a square mod p.
 * https://oeis.org/A038952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Generate primes p such that 69 is not a quadratic residue modulo p
   (include p = 2).  For odd primes, this is equivalent to Legendre symbol
   (69|p) = -1, which is checked via the Jacobi symbol `jacobi_symbol`.  The
   function keeps a static cache of terms so that calls in arbitrary order
   are O(1) after the initial computation. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/grow_seq.h"
#include "../utils/is_prime.h"
#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *vals = NULL;
    static size_t vals_len = 0, vals_cap = 0;

    /* state needed to continue searching primes across calls */
    static uint64_t cand = 2;     /* next candidate prime to test */
    static idx_t found = 0;       /* number of terms already found */

    /* Extend cache until we have at least n terms */
    while (found < n)
    {
        if (cand == 2ULL)          /* handle prime 2 separately */
        {
            grow_seq(&vals, &vals_cap, vals_len + 1);
            vals[vals_len++] = (seq_t)2;
            found++;
            cand = 3;              /* next candidate will be odd */
        }
        else                        /* cand is odd and ≥3 */
        {
            if (is_prime(cand) && jacobi_symbol(69, cand) == -1)
            {
                grow_seq(&vals, &vals_cap, vals_len + 1);
                vals[vals_len++] = (seq_t)cand;
                found++;
            }
            cand += 2;              /* skip even numbers */
        }
    }
    return vals[n - 1];
}
