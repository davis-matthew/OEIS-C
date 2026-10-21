/*
 * A094683  Juggler sequence: if n mod 2 = 0 then floor(sqrt(n)) else
 *          floor(n^(3/2)).
 * https://oeis.org/A094683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = floor(sqrt(n)) if n even * a(n) = floor(n*sqrt(n)) =
 *            floor(sqrt(n³)) if n odd * offset 0, so a(0) is the first term
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 2004, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* integer square root for seq_t (non‑negative). */
static seq_t seq_isqrt(seq_t v)
{
    seq_t lo = 0, hi = v;                 /* hi <= v, works for 0 */
    while (lo < hi) {
        /* mid = ceil((lo+hi)/2) to avoid infinite loop when lo+hi overflows */
        seq_t mid = (lo + hi + 1) >> 1;
        if (mid * mid <= v)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

extern seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;
    if ((n & 1ULL) == 0)            /* even n: floor(sqrt(n)) */
        return seq_isqrt(val);
    /* odd n: floor(n*sqrt(n)) == floor(sqrt(n³)) */
    seq_t val3 = val * val * val;
    return seq_isqrt(val3);
}
