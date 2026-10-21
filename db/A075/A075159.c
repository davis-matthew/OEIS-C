/*
 * A075159  Run lengths in the binary expansion of n-1 gives the vector of
 *          exponents in prime factorization of a(n), with the least
 *          significant run corresponding to the exponent of the least prime,
 *          2.
 * https://oeis.org/A075159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product over i of p_i ^ e_i, where e_i are the
 *            run–lengths of the binary expansion of n-1 after subtracting 1
 *            from all runs except the most significant one; p_i is the i‑th
 *            prime (p_0 = 2) and the least significant run corresponds to i =
 *            0
 * Range:     u8 n <= 57, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* special case: n = 1 --> a(1) = 1 */
    if (n <= 1) return 1;

    /* binary representation of n-1 */
    idx_t m = n - 1;
    /* store run lengths (least significant run first) */
    seq_t runs[64];
    size_t run_count = 0;

    int cur_bit = m & 1;
    size_t len = 0;
    while (m) {
        int bit = m & 1;
        if (bit == cur_bit) {
            len++;
        } else {
            runs[run_count++] = (seq_t)len;
            cur_bit = bit;
            len = 1;
        }
        m >>= 1;
    }
    /* last run */
    runs[run_count++] = (seq_t)len;

    /* subtract 1 from every run except the most significant one */
    for (size_t i = 0; i < run_count; ++i) {
        if (i != run_count - 1) {
            if (runs[i] > 0) runs[i] -= 1;
        }
    }

    /* first 20 primes – enough for n up to about 10^5 */
    static const uint32_t primes_u32[20] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
        31, 37, 41, 43, 47, 53, 59, 61, 67, 71
    };
    /* compute the product */
    seq_t result = 1;
    for (size_t i = 0; i < run_count; ++i) {
        seq_t e = runs[i];
        if (e == 0) continue;
        /* multiply by the i-th prime raised to e */
        seq_t p = (seq_t)primes_u32[i];
        for (seq_t j = 0; j < e; ++j) {
            result = seq_mul(result, p);
        }
    }
    return result;
}
