/*
 * A007790  a(n) = (1/n)*Sum_{x in (Z/nZ)^4} e^(2*Pi*i*det(circ(x))/n), the
 *          normalized Gauss sum of the 4 X 4 circulant determinant.
 * https://oeis.org/A007790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   multiplicative; a(2)=0, a(4)=2^5, a(8)=2^8, a(16)=2^11,
 *            a(2^k)=(k-4)*2^(2k+2) for k>=5; p == 1 (mod 4): a(p^k) =
 *            ((k+1)(k+2)/2*p^2 - k(k+2)*p + k(k+1)/2) * p^(2k-2); p == 3 (mod
 *            4): a(p^k) = p^k + Sum_{j=0..k-1}
 *            (p^2-1)*p^(2k-2j-2)*p^min(2j,k) (from a(p^k) = Sum_{w,z mod p^k}
 *            p^min(v_p(w^2+z^2),k...
 * Range:     u8 n <= 7, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   a_power_of_2, a_power_of_odd_prime, power_of_p
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a_power_of_2.h"
#include "../utils/a_power_of_odd_prime.h"
#include "../utils/power_of_p.h"

seq_t a(idx_t n) {
    if (n == 1) return 1;

    seq_t result = 1;
    idx_t temp = n;

    idx_t k2 = 0;
    while (temp % 2 == 0) {
        k2++;
        temp /= 2;
    }
    if (k2 > 0) {
        if (k2 == 1) return 0;
        result *= a_power_of_2(k2);
    }

    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t kp = 0;
            while (temp % p == 0) {
                kp++;
                temp /= p;
            }
            result *= a_power_of_odd_prime((seq_t)p, kp);
        }
    }

    if (temp > 1) {
        result *= a_power_of_odd_prime((seq_t)temp, 1);
    }

    return result;
}
