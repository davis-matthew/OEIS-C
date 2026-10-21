/*
 * A238642  a(n) = n if n+1 is prime; otherwise, a(n) = n * (smallest prime
 *          factor of n+1).
 * https://oeis.org/A238642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n if n+1 is prime; otherwise a(n) = n * (smallest prime
 *            factor of n+1)
 * Range:     u8 n <= 47, u16 n <= 1679, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _spf_of_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_spf_of_uint64.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Honor the OEIS offset: a(1) is the first term; for n < 1 return 0 */
    if (n == 0U) {
        return (seq_t)0;
    }

    uint64_t m   = n + 1U;                 /* n + 1  */
    uint64_t spf = _spf_of_uint64(m);     /* smallest prime factor of n+1 */

    if (spf == m) {                       /* m is prime */
        return (seq_t)n;
    } else {
        return (seq_t)n * (seq_t)spf;
    }
}
