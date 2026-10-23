/*
 * A024602  s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n+1-k), where k = [ (n+1)/2 ],
 *          s = (odd natural numbers), t = A014306.
 * https://oeis.org/A024602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=1}^{floor((n+1)/2)} (2*i-1)*t(n+1-i), where t(m) =
 *            1 if m is not a tetrahedral number, t(m) = 0 if m is a
 *            tetrahedral number. A tetrahedral number is T(r)=r(r+1)(r+2)/6
 * Range:     u8 n <= 32, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* k = floor((n+1)/2) */
    idx_t k = (n + 1) / 2;
    /* base sum of first k odd numbers */
    seq_t base = (seq_t)k * (seq_t)k;

    /* lower bound of indices that appear in the sum */
    idx_t low = n + 1 - k;   /* inclusive */

    seq_t sub = 0; /* sum of weights of tetrahedral indices in [low, n] */

    for (idx_t r = 1;; ++r) {
        seq_t tr = (seq_t)r;
        seq_t t_val = (tr * (tr + 1) * (tr + 2)) / 6;
        if (t_val > n) break;
        if (t_val >= low) {
            /* weight for index t_val: 2*(n - t_val) + 1 */
            seq_t diff = (seq_t)(n - t_val);
            seq_t weight = 2 * diff + 1;
            sub += weight;
        }
    }

    return base - sub;
}
