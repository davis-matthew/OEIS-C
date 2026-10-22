/*
 * A001777  Lah numbers: a(n) = n! * binomial(n-1, 4)/5!.
 * https://oeis.org/A001777
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * binomial(n-1, 4) / 5!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; i++) {
        fact *= (seq_t)i;
    }
    
    seq_t q1 = fact / 120;
    
    seq_t sn = (seq_t)n;
    seq_t prod = (sn - 1) * (sn - 2) * (sn - 3) * (sn - 4);
    seq_t q2 = prod / 24;
    
    return q1 * q2;
}
