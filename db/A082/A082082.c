/*
 * A082082  Number of steps to reach 1 in the process of expanding the
 *          interval (i1,i2) successively to right and left, such that i1 and
 *          i2 always stay coprime, starting with (n,n).
 * https://oeis.org/A082082
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed by simulating the interval‑expansion process.
 *            Start with (i1,i2) = (n,n). Alternate moving i2 upward to the
 *            next number coprime to i1, then moving i1 downward to the next
 *            number coprime to the new i2, until i1 becomes 1. Count the
 *            number of full (i2‑step, i1‑step) pairs performed
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
 
/* Simple Euclidean GCD for uint64_t.  Works for all build sizes
   because seq_t values are never larger than 64‑bit whenever we need
   them for coprimality testing in this function. */
static inline uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}
 
seq_t a(idx_t n)
{
    uint64_t i1 = (uint64_t)n;   /* left endpoint */
    uint64_t i2 = (uint64_t)n;   /* right endpoint */
    uint64_t steps = 0;          /* number of (i2,i1) upgrade cycles */
 
    while (i1 > 1) {
        /* Move i2 to the right until it is coprime with i1 */
        ++i2;
        while (gcd64(i1, i2) != 1) {
            ++i2;
        }
 
        /* Move i1 downwards until it is coprime with the new i2 */
        uint64_t new_i1 = i1 - 1;
        while (new_i1 > 0 && gcd64(new_i1, i2) != 1) {
            --new_i1;
        }
        i1 = new_i1;
 
        ++steps;
    }
 
    return (seq_t)steps;
}
