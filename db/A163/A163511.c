/*
 * A163511  a(0)=1. a(n) = p(A000120(n)) * Product_{m=1..A000120(n)}
 *          p(m)^A163510(n,m), where p(m) is the m-th prime.
 * https://oeis.org/A163511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = p(A000120(n)) * ∏_{m=1}^{A000120(n)} p(m)^{A163510(n,m)}
 *            * where p(k) is the k‑th prime. The implementation obtains the *
 *            binary positions of the set bits of n and uses a small table of
 *            * the first 20 primes. No external libraries are required
 * Range:     u8 n <= 34, u16 n <= 258, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint32_t primes_small[20] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
    23ULL, 29ULL, 31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL,
    59ULL, 61ULL, 67ULL, 71ULL
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */
    if (n == 1) return (seq_t)2;          /* a(1) = 2 */

    /* Determine the positions of the 1‑bits of n, in increasing order. */
    idx_t pos[64];
    idx_t cnt = 0;
    idx_t bitpos = 1;
    idx_t tmp = n;
    while (tmp) {
        if (tmp & 1ULL) {
            pos[cnt++] = bitpos;
        }
        ++bitpos;
        tmp >>= 1;
    }
    idx_t r = cnt;                         /* popcount of n */

    /* Result starts with the r‑th prime p(r). */
    seq_t result = (seq_t)primes_small[r - 1];
    idx_t prev = 0;

    for (idx_t i = 0; i < cnt; ++i) {
        idx_t curr = pos[i];
        idx_t exp = curr - prev - 1;       /* A163510(n,i+1) */
        if (exp) {
            seq_t base = (seq_t)primes_small[i]; /* p(i+1) */
            seq_t pow = 1;
            for (idx_t e = 0; e < exp; ++e) {
                pow *= base;               /* base^exp */
            }
            result *= pow;
        }
        prev = curr;
    }
    return result;
}
