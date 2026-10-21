/*
 * A070613  a(n) = n^5 mod 29.
 * https://oeis.org/A070613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 29
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute n^5 modulo 29. Works for any seq_t size,
   using only integer arithmetic and no external libs. */
seq_t a(idx_t n)
{
    /* modulo value fits in one byte, so safe to use literal 29 */
    const idx_t mod_idx = 29U;
    const seq_t mod = 29U;

    seq_t base = (seq_t)(n % mod_idx);   /* n modulo 29, cast to seq_t */
    seq_t result = 1;
    unsigned int exp = 5;                 /* exponent 5 */

    while (exp) {
        if (exp & 1U)
            result = (seq_t)((result * base) % mod);
        base = (seq_t)((base * base) % mod);
        exp >>= 1U;
    }

    return result;   /* already reduced modulo 29 */
}
