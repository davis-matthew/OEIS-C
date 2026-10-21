/*
 * A232779  Sum of iterated logs; a(n) = 0 if n = 0; otherwise n +
 *          a(floor(log_2(n))).
 * https://oeis.org/A232779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n = 0; otherwise a(n) = n + a(floor(log₂(n)))
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: compute floor(log₂(v)) for a 64‑bit unsigned index. */
static idx_t _floor_log2_u64(idx_t v)
{
    idx_t r = 0;
    while (v > 1) {
        v >>= 1;
        ++r;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sum = 0;
    idx_t cur = n;

    while (cur > 0) {
        sum += (seq_t)cur;
        cur = _floor_log2_u64(cur);
    }

    return sum;
}
