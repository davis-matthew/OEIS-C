/*
 * A055510  Largest odd prime in sequence obtained in 3x+1 (or Collatz)
 *          problem starting at n, or 0 if no such prime is found.
 * https://oeis.org/A055510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest odd prime appearing in the Collatz sequence
 *            starting from n (or 0 if none). The Collatz iteration is x → x/2
 *            if x even, otherwise x → 3x+1
 * Range:     u8 n <= 26, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The helper isPrimeGeneric is provided by the OEIS build; it
   correctly tests primality of a seq_t value. */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    seq_t x      = (seq_t)n;   /* start of the Collatz sequence */
    seq_t best   = 0;          /* largest odd prime seen so far */

    while (true)
    {
        /* Test odd numbers for primality */
        if ((x & 1) && isPrimeGeneric(x) && x > best)
            best = x;

        /* Stop once we reach 1 (which is not prime) */
        if (x == 1)
            break;

        /* Next Collatz step */
        if ((x & 1) == 0)      /* even */
            x >>= 1;
        else                   /* odd */
            x = 3 * x + 1;
    }

    return best;
}
