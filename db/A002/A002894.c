/*
 * A002894  a(n) = binomial(2n, n)^2.
 * https://oeis.org/A002894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = binomial(2n, n)^2. Recurrence: (n+1)^2*a(n+1) =
 *            4*(2n+1)^2*a(n)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t gcd_seq(seq_t a, seq_t b) {
    while (b != 0) {
        seq_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

seq_t a(idx_t n) {
    seq_t result = 1;
    
    for (idx_t i = 0; i < n; i++) {
        seq_t i_seq = (seq_t)i;
        seq_t two_i_plus_1 = 2 * i_seq + 1;
        seq_t factor = 4 * two_i_plus_1 * two_i_plus_1;
        seq_t divisor = (i_seq + 1) * (i_seq + 1);
        
        seq_t g = gcd_seq(result, divisor);
        result = (result / g) * factor / (divisor / g);
    }
    
    return result;
}
