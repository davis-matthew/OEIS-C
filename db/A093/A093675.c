/*
 * A093675  Primes of the form 7*10^k + 3*R_k, where R_k is the repunit
 *          (A002275) of length k.
 * https://oeis.org/A093675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (22·10^(k) – 1)/3 where k is the index of the term (k =
 *            0,1,2,…). Only the values that are prime are listed in the
 *            sequence. This implementation generates the numbers by the
 *            recurrence x₀ = 7, x_{i+1} = 10·x_i + 3 and tests each for
 *            primality
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* sequence is defined for n ≥ 1; undefined otherwise */
        return 0;

    seq_t val  = 7;      /* x₀ */
    idx_t  count = 0;

    while (1) {
        if (is_prime_big(val)) {
            ++count;
            if (count == n)
                return val;
        }

        /* generate next term: x_{i+1} = 10*x_i + 3 */
        seq_t next = val * 10 + 3;

        /* For unsigned small types overflow wraps around to a
           smaller value; stop if that occurs.  For arbitrary‑precision
           types this never happens. */
        if (next <= val)
            break;

        val = next;
    }

    /* If we exit the loop before reaching the requested term,
       the term does not fit in the current seq_t type. */
    return 0;
}
