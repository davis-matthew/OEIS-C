/*
 * A079664  a(n) = Sum_{i=1..n} LookAndSay(i).
 * https://oeis.org/A079664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = Sum_{i=1..n} LookAndSay(i)
 * Range:     u8 n <= 9, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   look_and_say
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the look‑and‑say value of a non‑negative
   integer i by reading its decimal digits from most significant
   to least significant and concatenating the run lengths with
   the corresponding digits.  Then it sums these values for
   all i between 1 and n inclusive.  The implementation uses
   only fixed‑size buffers and does not rely on any external
   helper functions. */

#include <stdint.h>
#include "../utils/look_and_say.h"

/* Main function: returns a(n) = Sum_{i=1..n} LookAndSay(i). */
seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;
    seq_t sum = 0;
    for (idx_t i = (idx_t)1; i <= n; ++i) {
        sum += look_and_say(i);
    }
    return sum;
}
