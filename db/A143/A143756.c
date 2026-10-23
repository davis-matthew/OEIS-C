/*
 * A143756  Aliquot sequence starting at 81.
 * https://oeis.org/A143756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 81,
   a(n+1) = A001065(a(n))   (sum of proper divisors of a(n)) */
#include <stdint.h>

/* Compute the sum of proper divisors of x (A001065). */
static seq_t aliquot_sum(seq_t x)
{
    if (x <= 1) return 0;
    uint64_t n = (uint64_t)x;          /* values are small for this sequence */
    seq_t sum = 0;
    for (uint64_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            uint64_t j = n / i;
            if (i != n) sum += (seq_t)i;           /* i is proper divisor */
            if (j != i && j != n) sum += (seq_t)j; /* second divisor, if distinct */
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t cur = 81;
    if (n == 0) return cur;
    for (idx_t i = 1; i <= n; ++i) {
        if (cur == 0) break;          /* remains 0 thereafter */
        cur = aliquot_sum(cur);
    }
    return cur;
}
