/*
 * A127705  a(n) = mu(n) + Sum_{k|n, k>1} (k+1)*mu(n/k), where mu = A008683.
 * https://oeis.org/A127705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(n)-mu(n) for n>1; a(1)=1
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9285 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/totient.h"

/* Möbius function for the divisor in the loop */
static inline int mu_of(idx_t d) {
    int n = (int)d;
    int sign = 1;
    int i = 2;
    while ((int64_t)i * i <= n) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) {
                n /= i;
                ++cnt;
                if (cnt > 1) return 0;        /* square factor */
            }
            sign = -sign;
        }
        ++i;
    }
    if (n > 1) sign = -sign;
    return sign;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    seq_t ph = totient(n);
    int mu = mu_of(n);          /* mu∈{-1,0,1} */
    if (mu >= 0)
        return ph - (seq_t)mu;  /* phi(n) - mu(n)  */
    else
        return ph + (seq_t)(-mu);
}
