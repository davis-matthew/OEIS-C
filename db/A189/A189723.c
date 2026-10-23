/*
 * A189723  Fixed point of the morphism 0->011, 1->101.
 * https://oeis.org/A189723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(3*k-2) = a(k), a(3*k-1) = 1 - a(k), a(3*k) = 1 (k >= 1), a(1)
 *            = 0
 * Range:     u8 n <= 122, u16 n <= 122, u32 n <= 122, u64 n <= 122,
 *            u128 n <= 122, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 1) {
        return 0;           /* base case: a(1) = 0 */
    }
    switch (n % 3) {
        case 0:			/* n = 3*k  */
            return 1;
        case 1:			/* n = 3*k-2  */
            return a((n + 2) / 3);
        case 2:			/* n = 3*k-1  */
            return 1 - a((n + 1) / 3);
        default:		/* unreachable */
            return 0;
    }
}
