/*
 * A005209  Multilevel sieve: at k-th step, accept k numbers, reject k, accept
 *          k, ...
 * https://oeis.org/A005209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve traced backwards. Position p (0-based) after step k came
 *            from position floor(p/k)*2k + (p mod k) after step k-1. Start
 *            with p = n-1 after step n, apply this for k = n..1, then a(n) =
 *            p + 1
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t p = (seq_t)(n - 1);
    for (idx_t k = n; k > 0; k--) {
        seq_t q = p / k;
        seq_t r = p % k;
        p = q * (seq_t)(2 * k) + r;
    }
    return p + 1;
}
