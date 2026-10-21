/*
 * A054372  Table of resultants for cyclotomic polynomials phi_k(x) and
 *          phi_n(x).
 * https://oeis.org/A054372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, tabl
 * Formula:   not stated
 * Range:     u8 n <= 216, u16 n <= 577, u32 n <= 2368, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   _smallest_prime_factor, big_pow, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A054372: resultant table of cyclotomic polynomials. 
   a(n) is obtained by mapping n to antidiagonal coordinates (i,j) with 
   i ≥ j ≥ 1:
     i is the smallest integer such that i*(i–1)/2 < n ≤ i*(i+1)/2,
     j = n – i*(i–1)/2 (hence 1 ≤ j ≤ i).
   The resultant formula:
     – if i == j          → 0
     – if j does not divide i → 1
     – else let r = i/j
       * if r is not a prime power → 1
       * otherwise let p be the prime base of r
         result = p^φ(j)  (φ = Euler’s totient)
   All arithmetic that could overflow a small seq_t is performed with
   idx_t, which is a 64‑bit unsigned integer. */

#include <stdint.h>
#include "../utils/phi.h"
#include "../utils/big_pow.h"
#include "../utils/_smallest_prime_factor.h"

seq_t a(idx_t n)
{
    /* Find i such that i*(i-1)/2 < n ≤ i*(i+1)/2 */
    idx_t low  = 1, high = 2;
    while (high * (high + 1) / 2 < n)   /* use idx_t to avoid overflow */
        high <<= 1;

    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t i = low;

    /* Compute j = n - i*(i-1)/2  (j ∈ [1,i]) */
    idx_t prevSum = (i - 1) * i / 2;
    idx_t j = n - prevSum;

    /* Diagonal entries */
    if (i == j)
        return 0;

    /* If j does not divide i → resultant = 1 */
    if (i % j != 0)
        return 1;

    /* r = i / j */
    idx_t r = i / j;
    if (r <= 1)
        return 1;           /* should not happen because i > j */

    /* Determine if r is a prime power */
    uint64_t p = _smallest_prime_factor((uint64_t)r);
    if (p == 0)          /* r is prime; the helper returns 0 for primes */
        p = r;
    while (r % p == 0)
        r /= p;
    if (r != 1)          /* more than one prime factor → result 1 */
        return 1;

    /* r is a prime power p^k.  Result = p^φ(j) */
    seq_t phi_j = phi(j);                     /* φ(j) fits in seq_t    */
    return big_pow((seq_t)p, phi_j);           /* p^φ(j) computed precisely */
}
