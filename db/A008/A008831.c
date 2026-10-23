/*
 * A008831  Discrete logarithm of n to the base 2 modulo 13.
 * https://oeis.org/A008831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   2^a(n) == n (mod 13)
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* discrete log base 2 modulo 13 for n ≥ 1.
       Returns k such that 2^k ≡ n (mod 13); for n ≡ 0 (mod 13) returns 0. */
    static const seq_t log[13] = {
        0, /* 0 mod 13 – undefined */
        0, /* n≡1  → 2^0 ≡ 1 */
        1, /* n≡2  → 2^1 ≡ 2 */
        4, /* n≡3  → 2^4 ≡ 3 */
        2, /* n≡4  → 2^2 ≡ 4 */
        9, /* n≡5  → 2^9 ≡ 5 */
        5, /* n≡6  → 2^5 ≡ 6 */
        11,/* n≡7  → 2^11≡ 7 */
        3, /* n≡8  → 2^3 ≡ 8 */
        8, /* n≡9  → 2^8 ≡ 9 */
        10,/* n≡10 → 2^10≡10 */
        7, /* n≡11 → 2^7 ≡11 */
        6  /* n≡12 → 2^6 ≡12 */
    };
    idx_t r = n % 13u;
    return log[(size_t)r];
}
