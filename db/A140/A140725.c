/*
 * A140725  Inverse binomial transform of (0 followed by A037481).
 * https://oeis.org/A140725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(2)=4; a(n)=a(n-1)+6*a(n-2) for n>2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 26 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the recurrence a(n) = a(n-1) + 6*a(n-2) with the given base terms.  */

#include <stdlib.h>

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    /* Handle the first three base cases directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;

    /* For n >= 3 we use the recurrence.  */
    /* prev2 holds a(n-2), prev1 holds a(n-1).  */
    seq_t prev2 = (seq_t)1;    /* a(1) */
    seq_t prev1 = (seq_t)4;    /* a(2) */
    seq_t curr;

    for (idx_t i = 3; i <= n; ++i) {
        curr = prev1 + mul_small(prev2, 6); /* a(i) = a(i-1) + 6*a(i-2) */
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
