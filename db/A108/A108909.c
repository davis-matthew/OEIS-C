/*
 * A108909  Sum of k-digit multiples of n where k is the number of digits in
 *          n.
 * https://oeis.org/A108909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n * sum_{t=ceil(10^{k-1}/n)}^{floor((10^k-1)/n)} t, where
 *            k = number of decimal digits of n
 * Range:     u8 n <= 9, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"

/* Pre‑computed powers of 10: p10digits[d] = 10^(d-1) for 1 ≤ d ≤ 20 */
static const uint64_t p10digits[21] = {
    0ULL,
    1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL, 100000ULL,
    1000000ULL, 10000000ULL, 100000000ULL, 1000000000ULL,
    10000000000ULL, 100000000000ULL, 1000000000000ULL,
    10000000000000ULL, 100000000000000ULL, 1000000000000000ULL,
    10000000000000000ULL, 100000000000000000ULL, 1000000000000000000ULL,
    10000000000000000000ULL   /* 10^19 */
};

/* Count decimal digits of a 64‑bit unsigned integer. */
static unsigned int num_digits_uint64(uint64_t x)
{
    for (int d = 20; d >= 1; --d) {
        if (x >= p10digits[d]) return d;
    }
    return 1;   /* never reached for x ≥ 1 */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t nseq  = (seq_t)n;
    unsigned int k = num_digits_uint64(n);

    seq_t lower = pow10(k - 1);   /* 10^{k-1} */
    seq_t upper = pow10(k);       /* 10^k     */

    seq_t tmin = (lower + nseq - 1) / nseq;         /* ceil(lower/n) */
    seq_t tmax = (upper - 1) / nseq;                /* floor((upper-1)/n) */
    seq_t count = tmax - tmin + 1;                  /* number of multiples */

    if (count == 0) return 0;                       /* safety, though it never happens for n≥1 */

    seq_t sum_t  = (tmin + tmax) * count / 2;       /* sum of the t‑range */
    return nseq * sum_t;                            /* final sum of k‑digit multiples */
}
