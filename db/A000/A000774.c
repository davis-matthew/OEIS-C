/*
 * A000774  a(n) = n!*(1 + Sum_{i=1..n} 1/i).
 * https://oeis.org/A000774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n!*(1 + Sum_{i=1..n} 1/i), recurrence: a(n+1) =
 *            (n+1)*a(n) + n!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    seq_t factorial = 1;
    
    for (idx_t i = 0; i < n; i++) {
        result = (seq_t)(i + 1) * result + factorial;
        factorial *= (seq_t)(i + 1);
    }
    
    return result;
}
