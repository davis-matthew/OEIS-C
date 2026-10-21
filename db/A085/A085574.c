/*
 * A085574  Least k such that n*(n+1)*(n+2) + k is palindromic.
 * https://oeis.org/A085574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest k≥0 such that n*(n+1)*(n+2)+k is a decimal
 *            palindrome
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   digits_to_seq, inc_middle, next_pal, seq_to_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/digits_to_seq.h"
#include "../utils/inc_middle.h"
#include "../utils/next_pal.h"
#include "../utils/seq_to_digits.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t base = sn * (sn + 1) * (sn + 2);

    /* if base itself is palindromic then k=0 */
    seq_t pal = next_pal(base);
    if (pal == base) return 0;
    return pal - base;
}
