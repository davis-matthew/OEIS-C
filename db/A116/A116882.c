/*
 * A116882  A number k is included if (highest odd divisor of k)^2 <= k.
 * https://oeis.org/A116882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth smallest positive integer k such that (highest
 *            odd divisor of k)^2 ≤ k. Equivalently, k = d·2^e with d odd and
 *            d ≤ 2^e
 * Range:     u8 n <= 15, u16 n <= 351, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t count_leq(seq_t x)
{
    seq_t sum = 0;
    seq_t pow2 = 1;
    while (pow2 <= x) {
        seq_t M     = x / pow2;           /* floor(x / 2^e)                */
        seq_t dmax  = (M < pow2) ? M : pow2; /* min(2^e, M)               */
        sum += (dmax + 1) / 2;    /* count of odd d ≤ dmax         */
        /* Stop when next power would exceed x, guarding against overflow.  */
        if (pow2 > x >> 1) break;
        pow2 <<= 1;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* the sequence starts at a(1)    */

    seq_t low  = 1;
    seq_t high = (seq_t)n * n;     /* a(n) ≈ n²/2;  n² is always ≥ a(n) */

    /* Ensure upper bound is large enough. */
    while (count_leq(high) < n) {
        high <<= 1;
    }

    /* Binary search for the smallest k with count_leq(k) ≥ n. */
    while (low < high) {
        seq_t mid = low + ((high - low) >> 1);
        if (count_leq(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}
