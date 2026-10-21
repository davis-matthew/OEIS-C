/*
 * A075065  a(1) = 1 and then alternately even and odd composite numbers
 *          matching the parity of the index.
 * https://oeis.org/A075065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1. For n > 1, if n is even -> a(n) is the (n/2)-th even
 *            composite number if n is odd -> a(n) is the ((n-1)/2)-th odd
 *            composite number (Composite = integer > 1 that is not prime)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* simple deterministic primality test for 64‑bit values */
static bool is_prime_uint64(uint64_t v)
{
    if (v < 2) return false;
    if (v % 2ULL == 0ULL) return v == 2ULL;
    for (uint64_t d = 3ULL; d * d <= v; d += 2ULL)
        if (v % d == 0ULL) return false;
    return true;
}

/* returns the k-th composite number of the given parity (+1 for even, -1 for odd) */
static seq_t kth_composite(idx_t k, int parity)
{
    seq_t cand = (parity > 0) ? 4 : 9;        /* first even and first odd composite */
    const seq_t step = 2;
    idx_t count = 0;

    while (1) {
        uint64_t u = (uint64_t)cand;          /* safe because values are small */
        if (!is_prime_uint64(u)) {           /* composite */
            ++count;
            if (count == k) return cand;
        }
        cand = cand + step;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    if ((n & 1) == 0) {                      /* even index */
        idx_t k = n >> 1;                    /* n/2 */
        return kth_composite(k, 1);           /* even composites */
    } else {                                 /* odd index > 1 */
        idx_t k = (n - 1) >> 1;              /* (n-1)/2 */
        return kth_composite(k, -1);          /* odd composites */
    }
}
