/*
 * A004719  Delete all 0's from n.
 * https://oeis.org/A004719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = if n <= 9 then n else (if n mod 10 = 0 then a(n/10) else
 *            10*a(floor(n/10)) + n mod 10)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 9) return (seq_t)n;
    
    idx_t last_digit = n % 10;
    if (last_digit == 0) {
        return a(n / 10);
    } else {
        return (seq_t)10 * a(n / 10) + (seq_t)last_digit;
    }
}
