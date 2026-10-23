/*
 * A165942  For a nonnegative integer n, define dsf(n) =
 *          n_1^{n_1}+n_2^{n_2}+n_3^{n_3} + n_m^{n_m}, where
 *          {n_1,n_2,n_3,...n_m} lists digits of n. Then starting with a(1) =
 *          3418, a(n+1) = dsf(a(n)).
 * https://oeis.org/A165942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=3418; a(n+1)=dsf(a(n)) where dsf(x) is the sum of each
 *            decimal digit d raised to the power d (with 0^0 defined as 0)
 * Range:     u8 none, u16 n <= 1, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Pre‑computed d^d for 0 ≤ d ≤ 9, with 0^0 = 0. */
static const seq_t POW10[10] = {
    0ULL,                /* 0^0 = 0 */
    1ULL,                /* 1^1 = 1 */
    4ULL,                /* 2^2 = 4 */
    27ULL,               /* 3^3 = 27 */
    256ULL,              /* 4^4 = 256 */
    3125ULL,             /* 5^5 = 3125 */
    46656ULL,            /* 6^6 = 46656 */
    117649ULL,           /* 7^7 = 117649 */
    262144ULL,           /* 8^8 = 262144 */
    387420489ULL         /* 9^9 = 387420489 */
};

/* Helper: compute dsf(x) as sum of digit powers. */
static seq_t dsf(seq_t x)
{
    seq_t sum = 0;
    /* All values encountered by this sequence fit in 64‑bit, so
       we may safely cast to uint64_t for digit extraction. */
    uint64_t ux = (uint64_t)x;
    if (ux == 0) return 0;
    while (ux > 0) {
        unsigned d = (unsigned)(ux % 10);
        sum += POW10[d];
        ux /= 10;
    }
    return sum;
}

/* Public function as required by the build. */
seq_t a(idx_t n)
{
    /* The sequence enters a cycle of length 3 after the first term:
       3418 → 16777500 → 2520413 → 3418 … */
    static const seq_t cycle[3] = {
        (seq_t)3418,
        (seq_t)16777500,
        (seq_t)2520413
    };
    return cycle[(n - 1) % 3];
}
