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
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_idx.h"

static seq_t a_power_of_2(idx_t k) {
    if (k == 1) return 0;
    if (k == 2) return 32;
    if (k == 3) return 256;
    if (k == 4) return 2048;
    return (seq_t)(k - 4) * ipow_idx(2, 2 * k + 2);
}

static seq_t a_power_of_odd_prime(seq_t p, idx_t k) {
    seq_t p2 = p * p;
    if (p % 4 == 1) {
        seq_t t1 = (seq_t)((k + 1) * (k + 2) / 2) * p2;
        seq_t t2 = (seq_t)(k * (k + 2)) * p;
        seq_t t3 = (seq_t)(k * (k + 1) / 2);
        return (t1 - t2 + t3) * ipow_idx(p, 2 * k - 2);
    } else {
        seq_t res = ipow_idx(p, k);
        for (idx_t j = 0; j < k; j++) {
            idx_t m = (2 * j < k) ? 2 * j : k;
            res += (p2 - 1) * ipow_idx(p, 2 * k - 2 * j - 2 + m);
        }
        return res;
    }
}

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
