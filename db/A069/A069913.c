/*
 * A069913  a(n) = Sum_{d|n} (d-1)*tau(n/d).
 * https://oeis.org/A069913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (d-1) * tau(n/d) (OEIS A069913)
 * Range:     u8 n <= 59, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* ------- helper: count the number of divisors tau(k) ------------------ */
static seq_t count_divisors(idx_t k)
{
    if (k == 0) return 0;              /* undefined, but never used for n>=1 */
    seq_t result = 1;
    for (idx_t p = 2; p * p <= k; ++p) {
        if (k % p == 0) {
            uint32_t exp = 0;
            while (k % p == 0) {
                k /= p;
                ++exp;
            }
            result *= (exp + 1);
        }
    }
    if (k > 1) /* remaining prime factor */
        result *= 2;
    return result;
}

/* ------- main sequence function --------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* offset is 1 ; values for n<1 are not used */

    seq_t sum = 0;
    /* compute floor(sqrt(n)) in a safe integer loop */
    idx_t sqrt_n = 0;
    for (idx_t i = 1; i * i <= n; ++i)
        sqrt_n = i;

    for (idx_t d = 1; d <= sqrt_n; ++d) {
        if (n % d == 0) {
            idx_t q = n / d;                 /* the paired divisor */
            seq_t tau_q  = count_divisors(q);
            sum += (seq_t)(d - 1) * tau_q;

            if (d != q) {                    /* handle the second divisor */
                seq_t tau_d  = count_divisors(d);
                sum += (seq_t)(q - 1) * tau_d;
            }
        }
    }
    return sum;
}
