/*
 * A003970  Möbius transform of A003960 (with alternating zeros omitted).
 * https://oeis.org/A003970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult, changed
 * Formula:   a(n) = Möbius transform of A003960 at odd indices only. For m =
 *            2n-1, a(n) is multiplicative with a(p^e) = (p-1)/2 *
 *            ((p+1)/2)^(e-1) for odd primes p
 * Range:     u8 n <= 260, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a000272_ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000272_ipow.h"


seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    idx_t m = 2 * n - 1;
    seq_t result = 1;
    
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            
            seq_t p_half_minus = (seq_t)((p - 1) / 2);
            seq_t p_half_plus = (seq_t)((p + 1) / 2);
            seq_t power = a000272_ipow(p_half_plus, e - 1);
            result *= p_half_minus * power;
        }
    }
    
    if (m > 1) {
        seq_t p_half_minus = (seq_t)((m - 1) / 2);
        result *= p_half_minus;
    }
    
    return result;
}
