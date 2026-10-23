/*
 * A037565  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,2,1.
 * https://oeis.org/A037565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=2; for n>1,
   a(n) = 8*a(n-1) + d_{n-1},
   where the repeated base‑8 digit pattern is [2,2,1]
   (so d_k = 2 for k%3==0 or 1, d_k = 1 for k%3==2). */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset unused, safe guard */
    if (n == 1) return 2;

    static const unsigned pat[3] = {2, 2, 1};

    seq_t res = 2;
    for (idx_t i = 2; i <= n; ++i) {
        unsigned d = pat[(i - 1) % 3];
        res = res * 8 + d;
    }
    return res;
}
