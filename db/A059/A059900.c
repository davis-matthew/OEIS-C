/*
 * A059900  Successively interleaved alternate bits in binary expansion of n
 *          gives vector of exponents in prime factorization of a(n).
 * https://oeis.org/A059900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A059900   – Successively interleaved alternate bits in binary expansion of n
    yields exponents in the prime factorisation of a(n).
    For prime index k (starting from 0, p_0 = 2), the exponent e_k is obtained
    by taking the bits of n whose positions satisfy
          i ≡ 2^k – 1  (mod 2^(k+1))
    and reading them as a binary number with the bit at position 2^k – 1 as
    the least significant bit.  The value a(n) is the product of each prime
    raised to its corresponding exponent.  */

#include "../utils/seq_pow.h"     /* for seq_pow   */
#include "../utils/seq_mul.h"     /* for seq_mul   */

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0)
        return (seq_t)1;

    /* Determine bit length of n */
    idx_t bitlen = 0;
    idx_t tmp = n;
    while (tmp) {
        ++bitlen;
        tmp >>= 1;
    }

    /* First several primes – enough for n up to 64‑bit. */
    static const idx_t primes[] = {
        2, 3, 5, 7, 11, 13, 17,
        19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61
    };
    const size_t num_primes = sizeof(primes) / sizeof(primes[0]);

    seq_t result = (seq_t)1;

    for (idx_t k = 0; k < num_primes; ++k) {
        /* Start position for this prime's bits: 2^k - 1   */
        idx_t start = ((idx_t)1 << k) - 1;
        if (start >= bitlen)           /* no further non‑zero exponents */
            break;

        /* Step between consecutive bits of this exponent: 2^(k+1) */
        idx_t step   = ((idx_t)1 << (k + 1));

        idx_t exponent = 0;
        idx_t multiplier = 1;
        idx_t pos = start;

        /* Collect bits to form the exponent */
        while (pos < bitlen) {
            if ( (n >> pos) & 1 )
                exponent |= multiplier;
            multiplier <<= 1;
            pos += step;
        }

        if (exponent != 0) {
            seq_t prime_val = (seq_t)primes[k];
            seq_t pow_val   = seq_pow(prime_val, exponent);
            result = seq_mul(result, pow_val);
        }
    }

    return result;
}
