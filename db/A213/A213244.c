/*
 * A213244  Number of nonzero elements in GF(2^n) that are 5th powers.
 * https://oeis.org/A213244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^n - 1) / gcd(2^n - 1, 5)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   seq_gcd, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of non‑zero elements in GF(2^n) that are 5th powers */
#include "../utils/seq_pow.h"
#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* M = 2^n - 1 */
    seq_t M = seq_pow((seq_t)2, n);
    M -= (seq_t)1;

    /* g = gcd(M, 5) */
    seq_t g = seq_gcd(M, (seq_t)5);

    /* result = M / g */
    return M / g;
}
