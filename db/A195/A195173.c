/*
 * A195173  a(n) = 5*n - floor(2n*r), where r=(1+sqrt(5))/2 (the golden
 *          ratio).
 * https://oeis.org/A195173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n - floor(n*sqrt(5))
 * Range:     u8 n <= 7, u16 n <= 483, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute integer square root of x: ⌊√x⌋ */
static inline seq_t isqrt(seq_t x)
{
    seq_t low = 0, high = x, mid, best = 0;
    while (low <= high) {
        mid = low + (high - low) / 2;
        if (mid == 0) { low = mid + 1; continue; }
        if (mid <= x / mid) {   /* mid*mid <= x without overflow */
            best = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return best;
}

/* OEIS sequence A195173: a(n) = 5*n - ⌊2*n*r⌋  with r = (1+√5)/2 */
seq_t a(idx_t n)
{
    seq_t ns   = (seq_t)n;            /* n in seq_t type */
    seq_t val  = (seq_t)5 * ns * ns;  /* 5*n^2, equals (n*√5)^2 */
    seq_t s    = isqrt(val);          /* ⌊n*√5⌋ = ⌊√(5*n^2)⌋ */
    seq_t four = (seq_t)4 * ns;       /* 4*n */
    return four - s;                  /* 4*n - ⌊n*√5⌋ = 5*n - ⌊2*n*r⌋ */
}
