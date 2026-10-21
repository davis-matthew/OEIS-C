/*
 * A216445  a(n) = !n mod n!!.
 * https://oeis.org/A216445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = !n mod n!!
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 19, u64 n <= 33, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* compute n!! in seq_t (may overflow 64 bits only in
       arbitrary‑precision builds; 64‑bit builds are never
       asked to evaluate terms larger than 64 bits). */
    seq_t m = 1;
    for (idx_t i = n; i > 1; i -= 2)
        m *= (seq_t)i;

    /* Special cases */
    if (n == 0 || n == 1)
        return 0;

    seq_t d0 = 1 % m;  /* !0 mod m */
    seq_t d1 = 0;      /* !1 mod m */

    for (idx_t k = 2; k <= n; ++k) {
        seq_t sum = (d0 + d1) % m;          /* keep modulo m */
        /* multiply using __int128 to avoid overflow */
        seq_t mul = (seq_t)(((unsigned __int128)(k - 1) * sum) % m);
        d0 = d1;
        d1 = mul;
    }
    return d1;  /* !n mod n!! */
}
