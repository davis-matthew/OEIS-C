/*
 * A117982  Trisection of A117981.
 * https://oeis.org/A117982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=0}^{3n} (-1)^{3n-k}·L(C(3n,k),3)·2^k This
 *            implementation uses the closed product form: a(n) = ∏_{i≥1, d_i
 *            = digit of n} f(d_i,i) (d_0=0 always) where f(0,i) = 1, f(1,i) =
 *            2^{3^i} – 1, f(2,i) = 1 + 2^{3^i} + 2^{2·3^i}
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* utilities needed: big integer exponentiation and multiplication */
#include <stdbool.h>
#include "../utils/seq_pow.h"   /* seq_pow(seq_t base, idx_t exp) */

seq_t a(idx_t n)
{
    /* Obtain base‑3 digits of n (least significant digit first).        */
    idx_t m = n;
    int digits[32];          /* enough for n up to 10⁴ (max 9 ternary digits) */
    int dlen = 0;
    while (m > 0) {
        digits[dlen++] = (int)(m % 3);
        m /= 3;
    }

    /* Length of 3·n in base 3 = number of digits of n + 1 (adding a leading 0). */
    int lenN = dlen + 1;
    seq_t pow2[32];          /* pow2[i] = 2^{3^i} for i = 0 … lenN‑1               */

    /* pow2[0] = 2^{3^0} = 2.  For i>0 use 2^{3^i} = (2^{3^{i‑1}})^3.          */
    pow2[0] = (seq_t)2;
    for (int i = 1; i < lenN; ++i) {
        pow2[i] = seq_pow(pow2[i - 1], 3);
    }

    seq_t result = (seq_t)1;

    /* For each position i ≥ 1 (the 0th digit of 3·n is always 0).          */
    for (int i = 1; i < lenN; ++i) {
        int digit = digits[i - 1];               /* because of the trailing zero   */

        if (digit == 1) {
            seq_t term = pow2[i] - (seq_t)1;     /* 2^{3^i} – 1                    */
            result = result * term;              /* operator* works for builtin and big int types */
        } else if (digit == 2) {
            seq_t term = pow2[i] + (seq_t)1;     /* start with 1 + 2^{3^i}           */
            term = term + pow2[i] * pow2[i];     /* add 2^{2·3^i}                    */
            result = result * term;
        }
        /* digit == 0 contributes a factor of 1 – nothing to do          */
    }

    return result;
}
