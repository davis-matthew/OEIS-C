/*
 * A079870  a(1)=1 and for n>1: ceiling(n^(1/Omega(n))), where Omega(n) is the
 *          total number of prime factors of n (A001222).
 * https://oeis.org/A079870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 (n=1); for n>1 a(n) = ceil(n^(1/Omega(n)))
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 1943, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/bigomega.h"

/* compute floor of k-th root of n: largest x such that x^k <= n */
static seq_t floor_kthroot(seq_t n, seq_t k)
{
    seq_t low = 1, high = n;
    while (low < high) {
        seq_t mid = (low + high + 1) / 2;          /* upper mid */
        seq_t p = big_pow(mid, k);
        if (p <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    seq_t omega = bigomega(n);     /* Ω(n) ≥ 1 for n>1 */
    seq_t root  = floor_kthroot((seq_t)n, omega);
    seq_t pow   = big_pow(root, omega);

    return (pow == (seq_t)n) ? root : root + 1;
}
