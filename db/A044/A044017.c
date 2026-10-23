/*
 * A044017  Numbers k such that 2 and 7 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A044017: 
   The numbers k such that the decimal representation of k contains
   the substrings "27" or "72" while k+1 does not.
   The implementation performs a straightforward linear search,
   which is fast enough for the required range (up to ~10⁴th term). */

#include <stdbool.h>

/* Return true iff x contains the adjacent digits 27 or 72. */
static bool has_27_or_72(seq_t x)
{
    seq_t prev = 10;               /* sentinel value larger than any digit 0–9 */
    while (x > 0) {
        seq_t cur = x % 10;
        if ((prev == 2 && cur == 7) || (prev == 7 && cur == 2))
            return true;
        prev = cur;
        x = x / 10;
    }
    return false;
}

/* External linkage as required by the exercise. */
seq_t a(idx_t n)
{
    if (n == 0)         /* offset is 1 – a(0) is undefined */
        return (seq_t)0;

    idx_t found = 0;
    seq_t candidate = 1;

    while (true) {
        if (has_27_or_72(candidate) && !has_27_or_72(candidate + 1)) {
            ++found;
            if (found == n)
                return candidate;
        }
        ++candidate;
    }
}
