/*
 * A137442  n^2 followed by smallest integer not yet listed.
 * https://oeis.org/A137442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = IFTE(n mod 2 == 1, ((n+1)/2)^2, (n/2) + round(sqrt(n/2)))
 * Range:     u8 n <= 30, u16 n <= 510, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute floor(sqrt(x)) for 0 <= x < 2^64.  */
static inline idx_t isqrt_idx(idx_t x) {
    /* The maximum integer whose square can fit in 64 bits is 0xFFFFFFFF. */
    idx_t low  = 0;
    idx_t high = 1ULL << 32;          /* exclusive upper bound */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        /* Use a narrow range so mid*mid never overflows uint64_t. */
        if (mid * mid <= x) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low - 1;
}

/* Return round(sqrt(x)) as a seq_t.  */
static inline seq_t round_sqrt_idx(idx_t x) {
    idx_t s = isqrt_idx(x);
    idx_t diff1 = x - s * s;          /* x - s^2  */
    idx_t diff2 = (s + 1) * (s + 1) - x; /* (s+1)^2 - x */
    return (diff1 <= diff2) ? (seq_t)s : (seq_t)(s + 1);
}

seq_t a(idx_t n) {
    if ((n & 1ULL) == 1ULL) {            /* odd n → square term */
        idx_t m = (n + 1) / 2;
        seq_t sm = (seq_t)m;
        return sm * sm;
    } else {                             /* even n → m + round(sqrt(m)) */
        idx_t m   = n / 2;
        seq_t sm  = (seq_t)m;
        seq_t r   = round_sqrt_idx(m);
        return sm + r;
    }
}
