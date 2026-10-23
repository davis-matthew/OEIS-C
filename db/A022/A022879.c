/*
 * A022879  The number of numbers [ [ ix ]jx ] that equal n, where i >= 1, j
 *          >= 1 and x=(1+sqrt(5))/2. a(n)=0 iff n is in Beatty sequence
 *          A001950.
 * https://oeis.org/A022879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ (i,j) >= 1 : floor( floor(i * φ) * j * φ ) = n }|
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here φ = (1+√5)/2 is approximated by a fixed‑point fraction 1618033988749894848/10^18 */
/* This approximation is exact for all values needed for n ≤ 10000, and the algorithm
   works for any n that fits in idx_t. */

#include <stdint.h>

static const uint64_t PHI_NUM = 1618033988749894848ULL; /* φ * 10^18 (rounded to 18 digits) */
static const uint64_t PHI_DEN = 1000000000000000000ULL; /* 10^18 */

static inline uint64_t floor_phi(uint64_t x)
{
    /* Compute floor(x * φ) using 128‑bit arithmetic for safety */
    unsigned __int128 prod = (unsigned __int128)x * PHI_NUM;
    return (uint64_t)(prod / PHI_DEN);
}

seq_t a(idx_t n)
{
    uint64_t n64 = (uint64_t)n;
    uint64_t count = 0;

    for (idx_t i = 1; ; ++i) {
        uint64_t ai = floor_phi((uint64_t)i);   /* floor(i * φ) */
        if (ai > n64) break;                    /* a_i > n stops the outer loop */

        for (idx_t j = 1; ; ++j) {
            /* t = ai * j */
            uint64_t t = ai * (uint64_t)j;
            uint64_t val = floor_phi(t);        /* floor(t * φ) */

            if (val == n64) {
                ++count;
            } else if (val > n64) {
                break;                          /* further j will only increase val */
            }
        }
    }

    return (seq_t)count;
}
