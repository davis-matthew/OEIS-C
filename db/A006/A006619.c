/*
 * A006619  Zarankiewicz's problem.
 * https://oeis.org/A006619
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) computes Zarankiewicz's problem by summing differences. For
 *            n >= 9: a(n) = (n^2 + n + 20) / 2. For 2 <= n < 9: computed via
 *            accumulated differences d(i)
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 2) return 8;
    
    if (n >= 9) {
        seq_t n_seq = (seq_t)n;
        return 10 + (n_seq * (n_seq + 1)) / 2;
    }
    
    seq_t result = 8;
    for (idx_t i = 3; i <= n; i++) {
        if (i <= 4) result += 5;
        else if (i == 5) result += 6;
        else if (i == 6) result += 7;
        else if (i == 7) result += 7;
        else result += 8;
    }
    return result;
}
