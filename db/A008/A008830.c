/*
 * A008830  Discrete logarithm of n to the base 2 modulo 11.
 * https://oeis.org/A008830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   2^a(n) == n (mod 11)
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    static bool init = false;
    static seq_t map[11];
    if (!init) {
        /* initialize mapping from residue (1..10) to exponent */
        for (int i = 0; i < 11; ++i) map[i] = 0;
        seq_t cur = 1;                 /* 2^0 mod 11 */
        for (int e = 0; e < 10; ++e) {
            map[(int)cur] = e;         /* map residue -> exponent */
            cur = (cur * 2) % 11;      /* next power of 2 modulo 11 */
        }
        init = true;
    }
    idx_t r = n % 11;
    return map[(int)r];
}
