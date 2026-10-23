/*
 * A117971  One-based position of the first 2 from the least significant digit
 *          in the ternary expansion of 2^n, or 0 if there are no 2's present.
 * https://oeis.org/A117971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if n is odd, a(n) = smallest k>1 such that 2^n ≡
 *            2·3^(k-1) (mod 3^k), a(n) = 0 if no such k exists (conjectured
 *            only for n=0,2,8)
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 9801, u64 n <= 9801,
 *            u128 n <= 9801, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n) {
    /* Special cases */
    if (n == 0u) return 0u;
    if (n & 1u)   return 1u;   /* odd n → LSB digit is 2 */

    /* Pre‑compute powers of 3 up to 3^80 (fits in 128‑bit). */
    static seq_t pow3[81];
    static idx_t pow3_pre = 0u;
    if (pow3_pre == 0u) {
        pow3[0] = (seq_t)1u;
        for (idx_t i = 1u; i <= 80u; ++i) {
            pow3[i] = pow3[i-1] * (seq_t)3u;
        }
        pow3_pre = 80u;
    }

    /* Find the first position of a digit 2 (starting from 1). */
    for (idx_t k = 2u; k <= 80u; ++k) {
        seq_t r = mod_pow((seq_t)2u, n, pow3[k]);          /* 2^n mod 3^k */
        seq_t t = (r / pow3[k-1]) % ((seq_t)3u);          /* k‑th digit */
        if (t == (seq_t)2u) return (seq_t)k;
    }
    return 0u;   /* no digit 2 found within computed range */
}
