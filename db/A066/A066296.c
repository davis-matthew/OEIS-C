/*
 * A066296  a(n) = floor(n^(1/omega(n))).
 * https://oeis.org/A066296
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^(1/omega(n))) for n >= 2 where omega(n) is the
 *            number of distinct prime factors of n
 * Range:     u8 n <= 32, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_distinct_primes, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_distinct_primes.h"
#include "../utils/seq_pow.h"

static seq_t kth_root(seq_t n, unsigned k)
{
    if (k <= 1)
        return n;                 /* n^(1/1) = n */

    seq_t lo = 1;
    seq_t hi = n;
    seq_t best = 1;

    while (lo <= hi) {
        seq_t mid = lo + (hi - lo) / 2;
        seq_t p = seq_pow(mid, (idx_t)k);

        if (p <= n) {
            best = mid;          /* mid^(k) <= n  => mid is a candidate */
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    if (n < 2)                     /* sequence defined for n >= 2 */
        return 0;

    /* Count distinct prime factors (omega(n)) of the 64‑bit integer n */
    int omega = count_distinct_primes((uint64_t)n);

    /* Safety: omega should be >= 1 for all n >= 2, but guard against zero */
    unsigned k = (omega > 0) ? (unsigned)omega : 1;

    return kth_root((seq_t)n, k);
}
