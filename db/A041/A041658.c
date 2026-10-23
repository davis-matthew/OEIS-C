/*
 * A041658  Numerators of continued fraction convergents to sqrt(348).
 * https://oeis.org/A041658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A041658: Numerators of continued fraction convergents to sqrt(348).
 * Recurrence: p_-1 = 1, p_0 = 18,
 *              p_n = a_n * p_{n-1} + p_{n-2}
 * where the continued fraction of sqrt(348) is 
 * [18; 1,1,1,8,1,1,1,36] repeating after the first term.
 * Thus for n >= 1:
 *   a_n = period[(n-1) % 8] with period = {1,1,1,8,1,1,1,36}.
 */

#include <stdint.h>
#include <stddef.h>

static const uint8_t period[8] = {1,1,1,8,1,1,1,36};

seq_t a(idx_t n) {
    /* Handle the base cases directly */
    if (n == 0) return (seq_t)18;
    /* We'll iterate from i = 1 up to n, keeping two previous
     * numerators: p_{i-1} (prev) and p_{i-2} (prevprev).
     */
    seq_t prevprev = (seq_t)1;           /* p_{-1} */
    seq_t prev     = (seq_t)18;          /* p_0   */
    for (idx_t i = 1; i <= n; ++i) {
        /* Compute the partial quotient a_i */
        seq_t a_i = (seq_t)((i == 0) ? 18 : period[(i-1) % 8]);
        /* Recurrence: p_i = a_i * prev + prevprev */
        seq_t curr = a_i * prev + prevprev;
        /* Shift for next iteration */
        prevprev = prev;
        prev     = curr;
    }
    return prev; /* prev now holds p_n */
}
