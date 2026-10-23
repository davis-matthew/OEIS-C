/*
 * A153011  Sum of proper divisors, minus the number of proper divisors, minus
 *          cototient of n, plus 1.
 * https://oeis.org/A153011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_of_proper_divisors(n) - number_of_proper_divisors(n)
 *            - cototient(n) + 1
 * Range:     u8 n <= 239, u16 n <= 9712, u32 n <= 9712, u64 n <= 9712,
 *            u128 n <= 9712, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1, but guard anyway */
    seq_t n_seq = (seq_t)n;

    seq_t sum_all = 1;                   /* σ(n) – product of σ(p^e) */
    seq_t num_divs = 1;                  /* τ(n) – product of (e + 1) */
    seq_t phi = n_seq;                   /* φ(n) – start with n */

    uint64_t temp = n;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            /* σ(p^exp) = 1 + p + p^2 + ... + p^exp */
            seq_t sigma_factor = 1;
            seq_t power = 1;
            for (uint64_t i = 0; i < exp; ++i) {
                power = power * (seq_t)p;
                sigma_factor = sigma_factor + power;
            }
            sum_all = sum_all * sigma_factor;
            num_divs = num_divs * (exp + 1);
            phi = phi - phi / (seq_t)p;  /* multiply by (p-1)/p once per distinct prime */
        }
    }
    if (temp > 1) {                      /* remaining prime factor p^1 */
        seq_t p = (seq_t)temp;
        seq_t sigma_factor = 1 + p;      /* 1 + p */
        sum_all = sum_all * sigma_factor;
        num_divs = num_divs * 2;         /* e = 1 → e + 1 = 2 */
        phi = phi - phi / p;
    }

    seq_t sum_proper = sum_all - n_seq;
    seq_t num_proper = num_divs - 1;
    seq_t cototient = n_seq - phi;

    return sum_proper - num_proper - cototient + (seq_t)1;
}
