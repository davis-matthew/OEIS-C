/*
 * A076409  Sum of the quadratic residues of prime(n).
 * https://oeis.org/A076409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Sum of the quadratic residues of prime(n). A076409: a(n) =
 *            sum_{x=1}^{p-1} x^2 mod p, where p = prime(n). For p ≡ 1 mod 4:
 *            a = k*p, k = (p-1)/4. For p ≡ 3 mod 4 (p>3): a = (k - b)*p, k =
 *            (p-3)/4, b = (h(-p)-1)/2, h(-p) = class number of Q(√-p). For p
 *            = 2 or 3: a = 1. The class number is computed...
 * Range:     u8 n <= 11, u16 n <= 97, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   class_number_neg_prime, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/class_number_neg_prime.h"

seq_t a(idx_t n)
{
    uint32_t p = get_prime(n);          /* p is prime(n), 1‑based */
    if (p <= 3)                         /* a(1)=a(2)=1 */
        return (seq_t)1;

    if (p % 4 == 1) {                   /* p = 4k+1 */
        seq_t k = (seq_t)((p - 1) / 4);
        return k * (seq_t)p;
    } else {                            /* p = 4k+3, p > 3 */
        seq_t k = (seq_t)((p - 3) / 4);
        uint64_t h = class_number_neg_prime(p);
        seq_t b = (seq_t)((h - 1) / 2);
        return (k - b) * (seq_t)p;
    }
}
