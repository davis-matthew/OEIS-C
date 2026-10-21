/*
 * A079871  a(1)=1 and for n>1: ceiling(n^(1/Omega(n)))^Omega(n), where
 *          Omega(n) is the total number of prime factors of n (A001222).
 * https://oeis.org/A079871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(n^(1/Omega(n)))^Omega(n) where Omega(n) is the total
 *            number of prime factors of n (with multiplicity)
 * Range:     u8 n <= 35, u16 n <= 2439, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, bigomega, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The formula requires the total number of prime factors of a number
    (Omega).  The utilities provided by the framework supply a function
    bigomega() that returns this count.  The ceiling of the w-th root of n
    is the smallest integer k such that k^w >= n; we find it by binary
    searching k.  For n > 1 and w = 1 the result is trivially n.  For w > 1
    we bound the search by sqrt(n)+1, since k^w >= n implies k <= sqrt(n). */

#include <stdint.h>
#include "../utils/bigomega.h"
#include "../utils/big_pow.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)1;
    }

    /* Omega(n) */
    seq_t w = bigomega(n);

    /* For prime powers the answer is just n itself. */
    if (w == (seq_t)1) {
        return (seq_t)n;
    }

    /* Target value as Seq_t */
    seq_t target = (seq_t)n;

    /* Upper bound for the w‑th root: sqrt(n)+1 */
    seq_t high_seq = floor_sqrt(target) + (seq_t)1;
    idx_t low  = 1;
    idx_t high = (idx_t)high_seq;

    /* Binary search for the smallest k with k^w >= n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        seq_t pow_mid = big_pow((seq_t)mid, w);
        if (pow_mid >= target) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }

    idx_t k = low;
    return big_pow((seq_t)k, w);
}
