/*
 * A008392  Crystal ball sequence for A_8 lattice.
 * https://oeis.org/A008392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 143/448*n^8 + 143/112*n^7 + 2431/480*n^6 + 429/40*n^5 +
 *            3355/192*n^4 + 297/16*n^3 + 22079/1680*n^2 + 761/140*n + 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 17, u64 n <= 294, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Use 128‑bit arithmetic to avoid intermediate overflow. */
    __uint128_t x = n;
    __uint128_t n2 = x * x;
    __uint128_t n3 = n2 * x;
    __uint128_t n4 = n3 * x;
    __uint128_t n5 = n4 * x;
    __uint128_t n6 = n5 * x;
    __uint128_t n7 = n6 * x;
    __uint128_t n8 = n7 * x;

    __uint128_t sum =
        (__uint128_t)2145 * n8 +
        (__uint128_t)8580  * n7 +
        (__uint128_t)34034 * n6 +
        (__uint128_t)72072 * n5 +
        (__uint128_t)117425 * n4 +
        (__uint128_t)124740 * n3 +
        (__uint128_t)88316  * n2 +
        (__uint128_t)36528  * x  +
        (__uint128_t)6720;

    return (seq_t)(sum / ((__uint128_t)6720));
}
