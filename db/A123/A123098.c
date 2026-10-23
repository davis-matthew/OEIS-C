/*
 * A123098  Multiplicative encoding of triangle formed by reading Pascal's
 *          triangle mod 2 (A047999).
 * https://oeis.org/A123098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=0..n} p(i+1)^(C(n,i) mod 2), where C(n,i) mod
 *            2 = 1 iff all bits of i are a subset of those of n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 14, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   get_prime, seq_mul, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k) */
#include "../utils/seq_one.h"     /* seq_t seq_one(void) */
#include "../utils/seq_mul.h"     /* seq_t seq_mul(seq_t a, seq_t b) */

/* Natural number sequence A123098 */
seq_t a(idx_t n)
{
    seq_t prod = seq_one();              /* start with 1 */
    /* iterate over all submasks of n; each submask corresponds to an i s.t.
       C(n,i) is odd (Lucas's theorem) */
    for (idx_t sub = n; ; sub = (sub - 1) & n) {
        /* prime(i+1) gives the (i+1)-th prime; cast to seq_t before
           multiplication. */
        seq_t prime = (seq_t)get_prime(sub + 1);
        prod = seq_mul(prod, prime);
        if (sub == 0) break;               /* finished all submasks */
    }
    return prod;
}
