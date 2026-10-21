/*
 * A074231  Numbers k such that Kronecker(8,k) = mu(gcd(8,k)).
 * https://oeis.org/A074231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n - cos(π*(n-1)/2) (n ≥ 1)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* cos(π*(n-1)/2) takes values 1,0,-1,0 cyclically for n-1 ≡ 0,1,2,3 (mod 4). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute 2*n as a signed integer to safely handle the subtraction of -1. */
    long long two_n = (long long)2 * (long long)n;
    idx_t k = (n - 1) & 3;   /* n-1 modulo 4 */

    long long cosv;
    switch (k) {
        case 0:  cosv =  1;  break;   /* cos = 1   */
        case 1:  cosv =  0;  break;   /* cos = 0   */
        case 2:  cosv = -1;  break;   /* cos = -1  */
        case 3:  cosv =  0;  break;   /* cos = 0   */
    }

    long long result = two_n - cosv;
    return (seq_t)result;
}
