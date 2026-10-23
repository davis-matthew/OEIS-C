/*
 * A118528  Start with 1 and repeatedly reverse the digits and add 11 to get
 *          the next term.
 * https://oeis.org/A118528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=1; for n≥2, a(n)=reverse(a(n‑1))+11. After the first step
 *            the sequence enters a 9‑term cycle: 12,32,34,54,56,76,78,98,100
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* The first term is special. */
    if (n == 1)
        return (seq_t)1;

    /* Cycle of length 9 starting at a(2). */
    static const seq_t cycle[9] = {
        (seq_t)12, (seq_t)32, (seq_t)34, (seq_t)54,
        (seq_t)56, (seq_t)76, (seq_t)78, (seq_t)98, (seq_t)100
    };

    /* Compute position within the cycle: (n-2)%9. */
    idx_t pos = (n - (idx_t)2) % (idx_t)9;
    return cycle[(size_t)pos];
}
