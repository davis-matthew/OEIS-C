/*
 * A003807  Order of simple Chevalley group A_2(q), q = prime power.
 * https://oeis.org/A003807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = q^3*(q^2-1)*(q^3-1)/gcd(3,q-1), where q = A000961(n+1) is
 *            the n-th prime power > 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   a3807_is_prime_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/a3807_is_prime_power.h"

seq_t a(idx_t n) {
    static uint64_t *pp = NULL;
    static idx_t cnt = 0;
    static idx_t cap = 0;
    uint64_t q;
    seq_t qs, q2, q3, f2, f3;

    if (n > cnt) {
        uint64_t k;
        if (n > cap) {
            idx_t newcap = cap ? cap * 2 : 1024;
            uint64_t *tmp;
            idx_t i;
            if (newcap < n) newcap = n + 128;
            tmp = (uint64_t *)malloc(newcap * sizeof(uint64_t));
            for (i = 0; i < cnt; i++) tmp[i] = pp[i];
            free(pp);
            pp = tmp;
            cap = newcap;
        }
        k = cnt ? pp[cnt - 1] + 1 : 2;
        while (cnt < n) {
            if (a3807_is_prime_power(k)) pp[cnt++] = k;
            k++;
        }
    }

    q = pp[n - 1];
    qs = (seq_t)q;
    q2 = qs * qs;
    q3 = q2 * qs;
    f2 = q2 - 1;
    f3 = q3 - 1;
    if ((q - 1) % 3 == 0) f2 /= 3; /* q^2-1 divisible by 3 when q = 1 mod 3 */
    return q3 * f2 * f3;
}
