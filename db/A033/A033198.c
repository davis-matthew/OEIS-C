/*
 * A033198  Discriminants of real quadratic number fields.
 * https://oeis.org/A033198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Sequence A033198: discriminants of real quadratic number fields.
   a(n) = the n-th square‑free integer d ≥ 2, mapped by
     a(n) = d   if d ≡ 1 (mod 4)
     a(n) = 4d  otherwise
*/

/* Helper: test whether an integer x is square‑free.
   Uses only integer arithmetic; the loop terminates early when
   the square of the divisor exceeds x. */
static bool is_squarefree(idx_t x)
{
    for (idx_t p = 2; (uint64_t)p * p <= (uint64_t)x; ++p) {
        uint64_t pp = (uint64_t)p * p;
        if ((uint64_t)x % pp == 0) {
            return false;
        }
    }
    return true;
}

/* Return the n-th discriminant of a real quadratic field. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n = 0 */

    idx_t val   = 2;      /* current candidate integer */
    idx_t count = 0;      /* number of square‑free integers found so far */

    for (;;) {
        if (is_squarefree(val)) {
            ++count;
            if (count == n) {
                /* apply the discriminant rule */
                if (val % 4 == 1) {
                    return (seq_t)val;
                } else {
                    return (seq_t)val * 4;
                }
            }
        }
        ++val;
    }
}
