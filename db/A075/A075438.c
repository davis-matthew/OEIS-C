/*
 * A075438  Triangle read by rows giving successive iterations of the Rule 60
 *          elementary cellular automaton starting with a single ON cell where
 *          row n is of length 2n+1.
 * https://oeis.org/A075438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 1 if binomial(floor(sqrt(n)), n - floor(sqrt(n))^2 -
 *            floor(sqrt(n))) is odd, otherwise 0. (All arithmetic modulo 2).
 *            This equals the flattened sequence of Rule 60 cellular automaton
 *            rows
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root: floor(sqrt(n)) for 64‑bit n */
static idx_t isqrt(idx_t n)
{
    idx_t low = 0, high = n;
    /* limit high to 0xFFFFFFFF to avoid overflow of sqrt(UINT64_MAX) */
    if (high > 0xFFFFFFFFULL) high = 0xFFFFFFFFULL;
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if (mid > n / mid)        /* mid*mid > n  (avoid overflow) */
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t r = isqrt(n);            /* r = ⌊√n⌋ */
    idx_t offset = n - r * r;      /* position within row r (0 .. 2r) */
    if (offset < r)                /* left zeros */
        return 0;
    idx_t k = offset - r;          /* index with respect to binomial (0 .. r) */
    /* binomial(r, k) is odd iff all bits of k are contained in r */
    if ((k & ~r) == 0)
        return 1;
    return 0;
}
