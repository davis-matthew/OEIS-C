/*
 * A083577  Primes in A003154.
 * https://oeis.org/A083577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 62, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A083577: primes of the form 12·T_m + 1, where T_m = m(m+1)/2 */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 */
    idx_t m     = 1;               /* triangular index (m ≥ 1) */
    idx_t found = 0;               /* number of primes found */

    while (1) {
        seq_t mm = (seq_t)m;                          /* promote to seq_t */
        seq_t p  = 6 * mm * (mm + 1) + 1;             /* 12*T_m + 1 = 6*m*(m+1)+1 */
        if (is_prime_big(p)) {                       /* primality test */
            ++found;
            if (found == n) return p;
        }
        ++m;
    }
}
