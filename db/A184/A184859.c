/*
 * A184859  Primes of the form floor(kr+h), where r=(1+sqrt(5))/2 and h=1/2.
 * https://oeis.org/A184859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th prime among U(k) = floor(k*(1+√5)/2 + 1/2) U(k)
 *            can be computed without floating point: U(k) = floor((k +
 *            floor_sqrt(5*k*k) + 1) / 2)
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/floor_sqrt.h"

/* compute U(k) = floor(k*φ + 1/2) where φ = (1+√5)/2
   using integer arithmetic: U(k) = floor((k + floor_sqrt(5*k*k) + 1) / 2) */
static seq_t U_value(idx_t k)
{
    /* 5*k*k may overflow a 64‑bit type, so do it in 128‑bit first. */
    unsigned __int128 prod = (unsigned __int128)5 * k * k;
    seq_t prod_s = (seq_t)prod;                     /* safe for all needed k */
    seq_t sqrt_v = floor_sqrt(prod_s);              /* floor(sqrt(5*k*k))   */
    return (k + sqrt_v + 1) / 2;                    /* integer division     */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                           /* undefined below offset */

    idx_t found = 0;
    idx_t k = 1;
    seq_t result = 0;

    while (found < n) {
        seq_t u = U_value(k);
        if (is_prime_big(u)) {
            ++found;
            if (found == n) {
                result = u;
                break;
            }
        }
        ++k;
    }
    return result;
}
