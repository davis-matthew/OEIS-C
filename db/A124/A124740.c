/*
 * A124740  a(n) = product of those positive integers which are coprime to
 *          both n and n+1 and which are <= n.
 * https://oeis.org/A124740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of integers k (1 ≤ k ≤ n) such that gcd(k, n) = 1
 *            and gcd(k, n + 1) = 1
 * Range:     u8 n <= 11, u16 n <= 15, u32 n <= 27, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   gcd, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t prod = (seq_t)1;          /* start with multiplicative identity */
    seq_t sn   = (seq_t)n;          /* cast once for efficiency           */
    seq_t sn1  = (seq_t)(n + 1);

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t sk = (seq_t)k;
        if (gcd(sk, sn) == (seq_t)1 && gcd(sk, sn1) == (seq_t)1)
            prod = seq_mul(prod, sk);
    }

    return prod;
}
