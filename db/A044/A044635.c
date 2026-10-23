/*
 * A044635  Numbers n such that string 0,3 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS sequence A044635.
   a(n) is the n‑th smallest positive integer such that
   the base‑9 representation of the integer contains the
   consecutive digits 0,3 ("03") while the representation of
   n+1 does not.  The sequence is offset 1 (a(1) = 84).  The
   function computes this by checking each integer in turn. */

/* Standard headers required by the build. */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Return true if x, written in base 9, contains the substring
   "03" (i.e., a digit 0 immediately followed by a digit 3). */
static bool contains_03(seq_t x)
{
    if (x == 0) return false;

    /* Digits are stored least‑significant first. */
    unsigned short digits[32];
    unsigned idx = 0;

    while (x > 0) {
        digits[idx++] = (unsigned short)(x % 9);
        x /= 9;
    }

    /* Look for the pattern 0,3 in the most‑significant → least‑significant
       direction.  In the array the more significant digit has the larger
       index, so we check digits[i] == 0 && digits[i-1] == 3. */
    for (unsigned i = 1; i < idx; ++i) {
        if (digits[i] == 0 && digits[i - 1] == 3) {
            return true;
        }
    }
    return false;
}

/* Main routine requested by the build system. */
seq_t a(idx_t n)
{
    /* n is 1‑based; any n < 1 is undefined, but we handle it
       defensively by returning 0. */
    if (n < 1) return 0;

    seq_t candidate = 0;
    idx_t count = 0;

    while (count < n) {
        if (contains_03(candidate) && !contains_03(candidate + 1)) {
            ++count;
            if (count == n) {
                return candidate;
            }
        }
        ++candidate;
    }
    return 0;   /* should never be reached for n >= 1 */
}
