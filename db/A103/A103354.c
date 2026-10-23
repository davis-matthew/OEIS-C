/*
 * A103354  a(n) = floor(x), where x is the solution to x = 2^(n-x).
 * https://oeis.org/A103354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(x), where x satisfies x = 2^(n-x)
 * Range:     u8 n <= 263, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline unsigned int floor_log2_idx(idx_t x)
{
    unsigned int r = 0;
    while (x > 1) {
        x >>= 1;
        r++;
    }
    return r;
}

static inline unsigned int ceil_log2_idx(idx_t x)
{
    return (x <= 1) ? 0 : floor_log2_idx(x - 1) + 1;
}

seq_t a(idx_t n)
{
    idx_t low = 1, high = n, best = 1;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned int ceil_val = ceil_log2_idx(mid);
        if (n - mid >= ceil_val) {
            best = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return (seq_t)best;
}
