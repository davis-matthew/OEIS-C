/*
 * A063201  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          18 ).
 * https://oeis.org/A063201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⌊5*n/6⌋ + ε(n), where ε(n) = 1 if n ≡ 3 (mod 6), -1 if n
 *            ≡ 4 (mod 6), 0 otherwise
 * Range:     u8 n <= 307, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute floor(5*n/6) using wider integer to avoid overflow */
    unsigned long long m = (unsigned long long)n;
    unsigned long long value = m * 5ULL / 6ULL;  /* floor division */
    seq_t res = (seq_t)value;                   /* cast back to seq_t */

    /* adjust for the residues 3 and 4 modulo 6 */
    switch (n % 6) {
        case 3: res += 1; break;
        case 4: res -= 1; break;
        default: break;
    }
    return res;
}
