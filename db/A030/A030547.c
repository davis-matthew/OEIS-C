/*
 * A030547  Number of terms (including the initial term) needed to reach a
 *          palindrome when the Reverse Then Add! map (x -> x +
 *          (x-with-digits-reversed)) is repeatedly applied to n, or -1 if a
 *          palindrome is never reached.
 * https://oeis.org/A030547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of terms required to reach a palindrome via the
 *            reverse‑then‑add procedure applied repeatedly to n. If no
 *            palindrome appears within 1000 iterations, a(n) = -1
 * Range:     u8 n <= 38, u16 n <= 88, u32 n <= 88, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   reverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/reverse.h"

static const idx_t MAX_ITER = 1000;

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    idx_t steps = 1;

    for (; steps <= MAX_ITER; ++steps) {
        seq_t rev = reverse(x);
        if (x == rev)
            return steps;          /* palindrome reached */
        x = x + rev;                /* reverse‑then‑add step */
    }
    return (seq_t)-1;               /* no palindrome within limit */
}
