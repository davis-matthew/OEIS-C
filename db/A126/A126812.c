/*
 * A126812  Ramanujan numbers (A000594) read mod 4.
 * https://oeis.org/A126812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 * sigma_7(n) (mod 4)
 * Range:     u8 n <= 9576, u16 n <= 9576, u32 n <= 9576, u64 n <= 9576,
 *            u128 n <= 9576, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n even -> n^2 ≡ 0 (mod 4) → a(n)=0 */
    if ((n & (idx_t)1) == 0) {
        return (seq_t)0;
    }

    /* n is odd; all its divisors are odd, so d^7 ≡ d (mod 4).
       For an odd d, d ≡ 1 or 3 (mod 4) and
       d^7 ≡ 1 or 3 (mod 4) respectively. */
    seq_t sum = 0;
    idx_t d = 1;
    while (d <= n / d) {
        if (n % d == 0) {
            /* divisor d */
            idx_t r = d & (idx_t)3;    // d mod 4
            sum += (r == 1 ? (seq_t)1 : (seq_t)3);

            /* complementary divisor e = n / d */
            idx_t e = n / d;
            if (e != d) {
                idx_t r2 = e & (idx_t)3;   // e mod 4
                sum += (r2 == 1 ? (seq_t)1 : (seq_t)3);
            }
        }
        ++d;
    }

    /* a(n) = sigma_7(n) mod 4 because n^2 ≡ 1 (mod 4) when n is odd */
    return sum & 3;
}
