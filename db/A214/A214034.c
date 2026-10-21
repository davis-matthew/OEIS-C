/*
 * A214034  The apparition sequence for 2,4 continued fractions.
 * https://oeis.org/A214034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 260, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A214034: Apparition sequence for 2,4 continued fractions.
   a(n) = z_U(n+1)-1, where U is defined by:
   U(0)=0, U(1)=1,
   U(k+1)=2*U(k)+U(k-1) if k is odd,
   U(k+1)=4*U(k)+U(k-1) if k is even.
   z_U(m) = smallest k≥1 with m | U(k).                     */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* modulus is n+1 */
    seq_t mod = (seq_t)(n + 1);

    /* current and previous terms of U modulo mod */
    seq_t u_prev = 0;
    seq_t u_curr = 1 % mod;   /* U(1) */

    idx_t k = 1;              /* index of u_curr */

    for (;;) {
        /* factor 2 or 4 depending on parity of k */
        seq_t factor = (k & 1) ? 2 : 4;

        /* compute U(k+1) modulo mod */
        seq_t next = (u_curr * factor + u_prev) % mod;

        /* if U(k+1) is zero, we have found z_U(mod) = k+1 */
        if (next == 0)
            return (seq_t)k;   /* a(n) = (k+1)-1 = k */

        /* advance state */
        u_prev = u_curr;
        u_curr = next;
        ++k;
    }
}
