/*
 * A006228  Expansion of e.g.f. exp(arcsin(x)).
 * https://oeis.org/A006228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1, a(n)=(1+(n-2)^2)*a(n-2) for n>=2
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0 || n == 1) return 1;
    
    seq_t result = 1;
    for (idx_t i = (n % 2 == 0) ? 2 : 3; i <= n; i += 2) {
        seq_t offset = (seq_t)(i - 2);
        seq_t mult = 1 + offset * offset;
        result = result * mult;
    }
    
    return result;
}
