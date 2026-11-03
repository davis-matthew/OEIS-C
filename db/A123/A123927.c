/*
 * A123927  Characteristic function of natural numbers with number of divisors
 *          equal to a Lucas number.
 * https://oeis.org/A123927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 if d(n) (number of divisors of n) is a Lucas number, 0
 *            otherwise
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 119 terms through n = 9989 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lucas numbers are defined by L0 = 2, L1 = 1, Lk+2 = Lk+1 + Lk */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/num_divisors.h"

/* test whether x is a Lucas number */
static bool is_lucas(seq_t x)
{
    if (x == (seq_t)1 || x == (seq_t)2)  /* Lucas numbers include 1 and 2 */
        return true;
    seq_t prev = (seq_t)2;   /* L0 */
    seq_t curr = (seq_t)1;   /* L1 */
    while (curr < x) {
        seq_t next = prev + curr;
        prev = curr;
        curr = next;
    }
    return curr == x;
}
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Count the total number of positive divisors of n. */
    uint64_t tau64 = num_divisors((uint64_t)n);
    seq_t tau = (seq_t)tau64;
    return is_lucas(tau) ? (seq_t)1 : (seq_t)0;
}
