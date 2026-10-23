/*
 * A028645  Expansion of (theta_3(z)*theta_3(20z)+theta_2(z)*theta_2(20z)).
 * https://oeis.org/A028645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions to x^2 + 20*y^2 = n with x ≡
 *            y (mod 2), counted with sign multiplicities: 1 for (0,0), 2 for
 *            one coordinate zero, 4 otherwise
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t isqrt_idx(idx_t v)
{
    idx_t low = 0, high = v, res = 0, mid;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= v) {
            res = mid;
            low = mid + 1;
        } else
            high = mid - 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t max_y = isqrt_idx(n / 20);
    for (idx_t y = 0; y <= max_y; ++y) {
        unsigned __int128 prod = (unsigned __int128)20 * y * y;
        if (prod > n) break;
        idx_t m = n - (idx_t)prod;
        idx_t x = isqrt_idx(m);
        if ((unsigned __int128)x * x != m) continue;
        if ((x & 1) != (y & 1)) continue;          /* parity test */
        if (x == 0 && y == 0)
            count += 1;
        else if (x == 0 || y == 0)
            count += 2;
        else
            count += 4;
    }
    return count;
}
