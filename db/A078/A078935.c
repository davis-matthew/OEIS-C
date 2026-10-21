/*
 * A078935  Largest divisor of n*(n+1)/2 that is <= sqrt(n*(n+1)/2).
 * https://oeis.org/A078935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the largest divisor of n*(n+1)/2 that does not exceed
 *            sqrt(n*(n+1)/2)
 * Range:     u8 n <= 10, u16 n <= 44, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t floor_sqrt_seq(seq_t v)
{
    seq_t low = 0;
    seq_t high = v;
    seq_t ans = 0;
    while (low <= high) {
        seq_t diff = high - low;
        seq_t mid = low + diff / 2;
        seq_t mid2 = mid * mid;
        if (mid2 == v) {
            return mid;
        }
        if (mid2 < v) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* Compute triangular number T = n*(n+1)/2 */
    seq_t T = (seq_t)n * ((seq_t)n + 1) / 2;

    /* Compute floor(sqrt(T)) */
    seq_t limit = floor_sqrt_seq(T);

    /* Search downward for the largest divisor <= limit */
    for (seq_t d = limit; d > 0; --d) {
        if (T % d == 0) {
            return d;
        }
    }
    return 1; /* T >= 1 so 1 will always divide */
}
