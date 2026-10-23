/*
 * A102672  Number of digits >= 3 in the decimal representations of all
 *          integers from 0 to n.
 * https://oeis.org/A102672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = ∑_{k≥0} [ 7*⌊n/10^(k+1)⌋*10^k + * (⌊n/10^k⌋%10>2 ?
 *            ((⌊n/10^k⌋%10-3)*10^k + (n%10^k)+1) : 0) ]
 * Range:     u8 n <= 185, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total = 0;
    idx_t pow = 1;
    while (pow <= n) {
        idx_t div_by_pow = n / pow;
        idx_t high       = div_by_pow / 10;
        idx_t cur        = div_by_pow % 10;
        idx_t low        = n % pow;

        seq_t contrib = (seq_t)high * 7 * (seq_t)pow;
        if (cur > 2) {
            contrib += (seq_t)(cur - 3) * (seq_t)pow + (seq_t)(low + 1);
        }
        total += contrib;

        if (pow > n / 10) break;   /* avoid overflow on next multiplication */
        pow *= 10;
    }
    return total;
}
