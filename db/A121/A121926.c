/*
 * A121926  a(n) = prime(n) + n!.
 * https://oeis.org/A121926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) + n!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   fact_upto, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/fact_upto.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* Offset starts at 1; return 0 for safety */

    seq_t prime = (seq_t)get_prime(n);   /* prime(n) : 1‑indexed */
    seq_t fact  = fact_upto(n);          /* n!   */

    return prime + fact;
}
