/*
 * A108622  Number of numerals to represent n in a base b multiplicative
 *          grouping numeral system where b=10.
 * https://oeis.org/A108622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = sum_{k=0}^{L-1} [d_k != 0] + sum_{k=1}^{L-1} [d_k > 1]
 *            where d_k are the decimal digits of n (units digit is k = 0)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    while (n > 0) {
        int r = n % 10;   /* units digit of current sub‑number */
        n /= 10;          /* remove the units digit */
        if (r != 0) ++cnt;          /* count the units numeral */
        if (n > 0) {                /* there is still a higher digit */
            int t = n % 10;         /* the next higher digit (tens in original n) */
            if (t > 1) ++cnt;       /* add the power symbol for this digit if >1 */
        }
    }
    return cnt;
}
