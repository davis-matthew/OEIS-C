/*
 * A160796  Total number of "ON" cells at n-th stage in simple 2-dimensional
 *          cellular automaton which is the "corner" structure corresponding
 *          to A160118.
 * https://oeis.org/A160796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recurrence for OEIS A160796
 *  a(0) = 0
 *  a(1) = 1
 *  a(2m)     = 4 * a(m) +
 *                4 * 3^(popcount(m)-1)   if m is odd
 *                0                         if m is even
 *  a(2m+1)   = a(2m) + 3^(popcount(m))
 *
 *  Here popcount(x) is the number of 1‑bits in the binary
 *  representation of x.
 *  The recurrence is derived from the relation with A160118.
 */

#include <stdint.h>

/* helper: popcount for idx_t (64‑bit) */
static unsigned int popcount_idx(idx_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll((unsigned long long)x);
#else
    unsigned int c = 0;
    while (x) { c += (unsigned int)(x & 1ULL); x >>= 1; }
    return c;
#endif
}

/* helper: compute 3^k (k <= 64) */
static seq_t pow3_int(unsigned int k)
{
    seq_t r = 1;
    for (unsigned int i = 0; i < k; ++i) {
        r *= 3;
    }
    return r;
}

/* recursive evaluation of a(n) */
static seq_t a_recursive(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    if ((n & 1u) == 0) {            /* even: n = 2*m */
        idx_t m = n >> 1;           /* m = n/2 */
        seq_t am = a_recursive(m);

        unsigned int pc = popcount_idx(m);
        seq_t add = 0;
        if (m & 1u)                 /* m odd: add 4*3^(pc-1) */
            add = (seq_t)4 * pow3_int(pc - 1);

        return (seq_t)4 * am + add;
    } else {                         /* odd: n = 2*m + 1 */
        idx_t m = (n - 1) >> 1;     /* m = (n-1)/2 */
        /* compute a(2m) using even rule */
        seq_t a_2m = a_recursive(2 * m);
        seq_t add = pow3_int(popcount_idx(m));
        return a_2m + add;
    }
}

/* public API as required */
seq_t a(idx_t n)
{
    return a_recursive(n);
}
