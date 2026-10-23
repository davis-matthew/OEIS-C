/*
 * A125139  SENSigma: Multiplicative with a(p^e) = p*(p^e-1)/(p-1) - (-1)^e.
 * https://oeis.org/A125139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e||n} ( (p^(e+1)-p)/(p-1) + (-1)^(e+1) )
 *            (equivalently a(p^e) = p + p^2 + … + p^e + (-1)^(e+1) )
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* out‑of‑bounds offset – undefined but safe */

    uint64_t num = (uint64_t)n;
    seq_t result = 1;

    /* Handle the prime 2 separately (faster trailing‑zero test) */
    if ((num & 1) == 0) {
        uint64_t exp = 0;
        while ((num & 1) == 0) {
            num >>= 1;
            exp++;
        }
        seq_t sum = 0;
        seq_t cur = 1;
        for (uint64_t i = 0; i < exp; ++i) {
            cur *= (seq_t)2;
            sum += cur;
        }
        if (((exp + 1) & 1) == 1)
            sum -= 1;               /* subtract 1 when e+1 is odd */
        else
            sum += 1;               /* add 1 when e+1 is even */
        result *= sum;
    }

    /* Odd prime factors */
    for (uint64_t p = 3; p * p <= num; p += 2) {
        if (num % p == 0) {
            uint64_t exp = 0;
            while (num % p == 0) {
                num /= p;
                exp++;
            }
            seq_t sum = 0;
            seq_t cur = 1;
            for (uint64_t i = 0; i < exp; ++i) {
                cur *= (seq_t)p;
                sum += cur;
            }
            if (((exp + 1) & 1) == 1)
                sum -= 1;
            else
                sum += 1;
            result *= sum;
        }
    }

    /* Remaining prime factor (if any) */
    if (num > 1) {
        uint64_t exp = 1;
        seq_t sum = 0;
        seq_t cur = 1;
        for (uint64_t i = 0; i < exp; ++i) {
            cur *= (seq_t)num;
            sum += cur;
        }
        if (((exp + 1) & 1) == 1)
            sum -= 1;
        else
            sum += 1;
        result *= sum;
    }

    return result;
}
