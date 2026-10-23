/*
 * A138436  a(n) = ((n-th prime)^5-(n-th prime)^3)/8.
 * https://oeis.org/A138436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (p_n^5 - p_n^3)/8 where p_n is the n‑th prime
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 31, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Uses get_prime from utils to obtain p_n as a uint32_t.  For
    64‑bit builds the intermediate calculations are performed in
    unsigned __int128 to avoid overflow before rounding down to seq_t. */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined below offset – return 0 */

    uint32_t p32 = get_prime(n);          /* p_n as 32‑bit value */
    seq_t p = (seq_t)p32;                 /* promote to seq_t */

    /* Use 128‑bit arithmetic for the intermediate powers. */
    __uint128_t p128 = (__uint128_t)p32;
    __uint128_t p2   = p128 * p128;       /* p^2   */
    __uint128_t p3   = p2 * p128;         /* p^3   */
    __uint128_t p5   = p3 * p2;           /* p^5   */

    __uint128_t num  = p5 - p3;           /* p^5 - p^3 */
    seq_t result = (seq_t)(num / 8ULL);    /* divide by 8 and cast back */

    return result;
}
