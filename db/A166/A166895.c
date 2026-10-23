/*
 * A166895  a(n) = Sum_{k=0..[n/2]} C(n-k,k)^(n-k)*n/(n-k), n>=1.
 * https://oeis.org/A166895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} C(n-k,k)^(n-k) * n/(n-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, binom_n_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_n_k.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t k = 0; k <= n / 2; ++k)
    {
        idx_t m = n - k;                     // exponent
        seq_t bin = binom_n_k(m, k);          // C(m, k)
        seq_t term = big_pow(bin, (seq_t)m);  // bin^(m)
        term = term * (seq_t)n;               // * n
        term = term / (seq_t)m;               // / (n-k)  (exact integer division)
        result = result + term;               // accumulate
    }
    return result;
}
