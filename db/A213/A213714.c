/*
 * A213714  Inverse function for injection A005187.
 * https://oeis.org/A213714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0; a(n) = k if A005187(k) = n for some k, else 0, where
 *            A005187(k) = 2k - popcount(k) = Sum_{j>=0} floor(k/2^j).
 *            Computed by binary search, since A005187 is strictly increasing
 * Range:     u8 n <= 403, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 107 terms through n = 6790 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A005187_a(idx_t n) {
    seq_t result = 0;
    idx_t power = n;
    while (power > 0) {
        result += (seq_t)power;
        power >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    /* A005187(k) >= k, so any preimage k lies in [1, n] */
    idx_t lo = 1, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        seq_t v = A005187_a(mid);
        if (v == (seq_t)n) return (seq_t)mid;
        if (v < (seq_t)n) lo = mid + 1;
        else hi = mid - 1;   /* mid >= 1, so no underflow */
    }
    return (seq_t)0;
}
