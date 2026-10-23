/*
 * A037841  a(n)=Sum{|d(i)-d(i-1)|: i=1,...,m}, where Sum{d(i)*9^i:
 *          i=0,1,...,m} is the base 9 representation of n.
 * https://oeis.org/A037841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{i=1}^{m} |d_i - d_{i-1}| where n = Σ_{i=0}^{m} d_i *
 *            9^i in base 9
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 8)        /* single base‑9 digit: no variation */
        return 0;

    seq_t sum = 0;
    idx_t prev_digit = n % 9;
    n /= 9;
    while (n > 0) {
        idx_t digit = n % 9;
        seq_t diff = (seq_t)((prev_digit > digit) ? (prev_digit - digit)
                                            : (digit - prev_digit));
        sum += diff;
        prev_digit = digit;
        n /= 9;
    }
    return sum;
}
