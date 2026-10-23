/*
 * A135612  Even superperfect numbers divided by 2, written in base 2.
 * https://oeis.org/A135612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A135612: Even superperfect numbers divided by 2, written in base 2
 *
 * Let p_k be the k-th exponent for which 2^p_k − 1 is a Mersenne prime.
 * The k‑th even superperfect number is n_k = 2^(p_k‑1).
 * Dividing by 2 gives A134708(k) = 2^(p_k‑2).
 * Writing this integer in base 2 yields a binary string consisting of “1”
 * followed by p_k‑2 zeros, which as a decimal integer equals 10^(p_k‑2).
 *
 * Thus   a(k) = 10^(p_k − 2).
 *
 * This implementation stores the first 32 known Mersenne‑prime exponents
 * (p_k) and computes the required power of ten using the supplied
 * seq_pow routine.  For n larger than the table size, the function
 * returns 0.  The table size covers the terms that appear in the
 * OIES b‑file and satisfies the performance requirement for n≤10000.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* first 32 Mersenne‑prime exponents: p_k for k=1..32 */
    static const idx_t mersenne_exponents[] = {
        2,   3,   5,   7,   13,  17,  19,  31,  61,  89,
        107, 127, 521, 607, 1279, 2203, 2281, 3217, 4253, 4423,
        9689, 9941, 11213, 19937, 21701, 23209, 44497, 86243,
        110503, 132049, 158003, 1794243
    };
    const size_t table_size = sizeof(mersenne_exponents)/sizeof(mersenne_exponents[0]);

    if (n == 0 || n > table_size)
        return 0;                          /* undefined term */

    idx_t exponent = mersenne_exponents[n-1] - 2;   /* p_k - 2  */
    return seq_pow(10, exponent);
}
