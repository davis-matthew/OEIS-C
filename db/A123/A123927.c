/*
 * A123927  Characteristic function of natural numbers with number of divisors
 *          equal to a Lucas number.
 * https://oeis.org/A123927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 if d(n) (number of divisors of n) is a Lucas number, 0
 *            otherwise
 * Range:     u8 n <= 9989, u16 n <= 9989, u32 n <= 9989, u64 n <= 9989,
 *            u128 n <= 9989, big any n
 * Helpers:   divisor_count_u64, is_lucas
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lucas numbers are defined by L0 = 2, L1 = 1, Lk+2 = Lk+1 + Lk */

#include "../utils/divisor_count_u64.h"
#include <stdbool.h>
#include "../utils/is_lucas.h"

seq_t a(idx_t n)
{
    /* Count the total number of positive divisors of n. */
    uint64_t tau64 = divisor_count_u64((uint64_t)n);
    seq_t tau = (seq_t)tau64;
    return is_lucas(tau) ? (seq_t)1 : (seq_t)0;
}
