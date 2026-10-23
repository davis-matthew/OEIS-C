/*
 * A028394  Iterate the map in A006369 starting at 8.
 * https://oeis.org/A028394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   iterate the map in A006369 starting at 8. Map: n -> if n mod 3 =
 *            0 then 2*n/3 elif n mod 3 = 1 then (4*n-1)/3 else (4*n+1)/3
 * Range:     u8 n <= 21, u16 n <= 69, u32 n <= 356, u64 n <= 681,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Use a 128‑bit unsigned integer for intermediate values
       to avoid overflow during the computation. */
    __uint128_t x = 8;
    for (idx_t i = 0; i < n; ++i) {
        unsigned r = (unsigned)(x % 3);   /* current residue mod 3 */
        __uint128_t k = x / 3;           /* floor(x / 3) */

        /* compute the next term without overflowing the 64‑bit type */
        if (r == 0)
            x = k * 2;
        else if (r == 1)
            x = k * 4 + 1;
        else          /* r == 2 */
            x = k * 4 + 3;
    }
    return (seq_t)x;   /* final value fits in seq_t, by definition of the sequence */
}
