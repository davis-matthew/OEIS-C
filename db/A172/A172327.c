/*
 * A172327  Floor(n*(sqrt(11)+sqrt(5))).
 * https://oeis.org/A172327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = floor(n*(sqrt(11)+sqrt(5)))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 45, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n > 0, a(n) lies between 5*n and 6*n.  We binary‑search this
   interval using only integer arithmetic.  The test “m <= a(n)”
   is equivalent to “(m/n) <= sqrt(11)+sqrt(5)”, which, by the
   minimal polynomial x⁴-32x²+36=0 of the constant, reduces to the
   integer inequality 32*m²*n² > m⁴ + 36*n⁴.  This comparison works
   for unsigned seq_t as well.  The routine is valid for any
   seq_t/idx_t combination provided by the build (including an
   arbitrary‑precision integer class). */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t n_seq = (seq_t)n;

    /* 5 < sqrt(11)+sqrt(5) < 6  */
    seq_t low  = (seq_t)5 * n_seq;
    seq_t high = (seq_t)6 * n_seq;
    seq_t best = low;

    while (low <= high) {
        seq_t mid = low + ((high - low) >> 1);

        /* m = mid.  Test if m <= a(n). */
        seq_t m2  = mid * mid;
        seq_t m4  = m2 * m2;
        seq_t n2  = n_seq * n_seq;
        seq_t n4  = n2 * n2;

        seq_t left  = (seq_t)32 * m2 * n2;          /* 32*m²*n²   */
        seq_t right = m4 + (seq_t)36 * n4;          /* m⁴ + 36*n⁴ */

        if (left > right) {          /* mid <= floor(n*α) */
            best = mid;
            low  = mid + (seq_t)1;
        } else {
            if (mid == (seq_t)0)
                break;                /* guard, not reached for n>0 */
            high = mid - (seq_t)1;
        }
    }

    return best;
}
