/*
 * A004515  Generalized nim sum n + n in base 5.
 * https://oeis.org/A004515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = generalized nim sum n + n in base 5 Convert n to base 5,
 *            apply digit mapping (0->0, 1->2, 2->4, 3->1, 4->3), convert back
 * Range:     u8 n <= 129, u16 n <= 9825, u32 n <= 9825, u64 n <= 9825,
 *            u128 n <= 9825, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t power = 1;
    static const int mapping[5] = {0, 2, 4, 1, 3};
    
    while (n > 0) {
        int digit = (int)(n % 5);
        result += (seq_t)mapping[digit] * power;
        power *= 5;
        n /= 5;
    }
    
    return result;
}
