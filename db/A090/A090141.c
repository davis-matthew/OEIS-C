/*
 * A090141  a(n) = Mod[n,4+Sum[Mod[Floor[n/4^m],4],{m,1,10}]].
 * https://oeis.org/A090141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = Mod[n, 4 + Sum[Mod[Floor[n/4^m], 4], {m, 1, 10}]]
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* may be predefined by the build */

seq_t a(idx_t n)
{
    /* We need to compute D = 4 + Σ_{m=1}^{10} ( floor(n / 4^m) mod 4 )
       and return n mod D.  D is always between 4 and 44, so it fits in
       any integer type.  The intermediate arithmetic uses idx_t to
       avoid overflow. */
    idx_t denom = 4;
    idx_t pow4  = 4;                /* 4^1 */
    for (int m = 1; m <= 10; ++m) {
        idx_t tmp = n / pow4;       /* floor(n / 4^m) */
        denom += (idx_t)(tmp % 4);  /* add (tmp mod 4) */
        pow4 *= 4;                  /* next power of 4 */
    }

    idx_t rem = n % denom;          /* remainder within [0, denom-1] */
    return (seq_t)rem;              /* cast to seq_t (will be small) */
}
