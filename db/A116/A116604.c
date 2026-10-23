/*
 * A116604  Expansion of q^(-1/2) * eta(q)^3 * eta(q^4) * eta(q^12) /
 *          (eta(q^2)^2 * eta(q^3)) in powers of q.
 * https://oeis.org/A116604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = b(2*n+1) where b(m) is a multiplicative function on odd *
 *            integers m defined by: * b(3^e) = -1 + 2*(-1)^e (e ≥ 0) * b(p^e)
 *            for p≡1,5 (mod 12) = e + 1 * b(p^e) for p≡7,11 (mod 12) = (1 +
 *            (-1)^e) / 2 (0 if e is odd) * b(m) is the product of the values
 *            on the prime‑power factors of m * (m odd...
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* m = 2*n + 1  (odd integer) */
    uint64_t m = 2U * n + 1U;

    /* factor m into primes p_i^e_i */
    uint64_t prime[64], exp[64];
    size_t cnt = 0;

    uint64_t tmp = m;
    /* trial division – m ≤ 2*10^4 for the required test range,
       so this is inexpensive. */
    for (uint64_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            prime[cnt] = p;
            exp[cnt]   = e;
            ++cnt;
        }
    }
    if (tmp > 1) {          /* remaining prime factor */
        prime[cnt] = tmp;
        exp[cnt]   = 1;
        ++cnt;
    }

    /* compute multiplicative value b(m) */
    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = prime[i];
        uint64_t e = exp[i];
        seq_t val;

        if (p == 3U) {                 /* special rule for 3^e */
            val = (e % 2 == 0) ? (seq_t)1 : (seq_t)-3;
        } else {
            uint64_t r = p % 12U;      /* p mod 12 for other primes */
            if (r == 1 || r == 5) {    /* p ≡ 1 or 5 (mod 12) */
                val = (seq_t)(e + 1);
            } else if (r == 7 || r == 11) { /* p ≡ 7 or 11 (mod 12) */
                val = (e % 2 == 0) ? (seq_t)1 : (seq_t)0;
            } else {                   /* should not happen for odd primes !=3 */
                val = (seq_t)1;
            }
        }

        prod *= val;
        if (prod == 0) break;         /* zero propagates */
    }

    return prod;
}
