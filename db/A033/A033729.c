/*
 * A033729  Expansion of Product_{d | 17} theta_3(q^d).
 * https://oeis.org/A033729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions (m,k) to m^2 + 17*k^2 = n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t isqrt_u64(idx_t n) {
    if (n == 0) return 0;
    idx_t low = 1, high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (mid <= n / mid) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t N) {
    seq_t count = 0;
    idx_t max_k = isqrt_u64(N / 17);
    for (idx_t k = 0; k <= max_k; ++k) {
        idx_t prod = 17 * k * k;
        if (prod > N) break;
        idx_t t = N - prod;
        if (t == 0) {
            if (k == 0) {
                count += 1;          /* (0,0) */
            } else {
                count += 2;          /* (0,±k) */
            }
            continue;
        }
        idx_t m = isqrt_u64(t);
        if (m * m != t) continue;
        if (k == 0) {
            count += 2;              /* (±m,0) */
        } else {
            count += 4;              /* (±m,±k) */
        }
    }
    return count;
}
