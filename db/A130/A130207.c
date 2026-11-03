/*
 * A130207  Diagonalized matrix of A000010, Euler totient function phi.
 * https://oeis.org/A130207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = phi(k) if n = k*(k+1)/2 else 0
 * Range:     u8 n <= 22155, u16 n <= 22155, u32 n <= 22155, u64 n <= 22155,
 *            u128 n <= 22155, big any n
 * Validated: standard, pass, 121 terms through n = 9487 (2026-11-01)
 * Helpers:   isqrt, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt.h"
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, safe guard       */
    idx_t t = 8*n + 1;             /* 8n+1 = (2k+1)^2 for triangular n     */
    idx_t s = isqrt(t);         /* floor sqrt of 8n+1                     */
    if (s*s != t)                  /* not a perfect square → not triangular  */
        return 0;
    idx_t k = (s - 1) / 2;         /* k such that n = k(k+1)/2               */
    return totient(k);                 /* Euler totient of k                     */
}
