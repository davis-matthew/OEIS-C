/*
 * A135282  Largest k such that 2^k appears in the trajectory of the Collatz
 *          3x+1 sequence started at n.
 * https://oeis.org/A135282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest k such that 2^k appears in the Collatz 3x+1
 *            trajectory starting at n. The function follows the trajectory of
 *            n, records the largest exponent of an exact power‑of‑two that
 *            occurs, and returns it
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t, is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_power_of_two.h"
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;          /* current term of the trajectory   */
    idx_t  max_exp = 0;          /* highest exponent found so far    */

    for (;;) {
        /* If x itself is a power of two, update max_exp. */
        if (is_power_of_two(x)) {
            idx_t exp = (idx_t)(bitlen_seq_t(x) - 1);
            if (exp > max_exp) max_exp = exp;
        }

        /* Termination when reaching 1. */
        if (x == (seq_t)1) break;

        /* Next Collatz step. */
        if ((x & (seq_t)1) == 0) {          /* even */
            x >>= 1;
        } else {                            /* odd  */
            x = 3 * x + 1;
        }
    }

    return (seq_t)max_exp;
}
