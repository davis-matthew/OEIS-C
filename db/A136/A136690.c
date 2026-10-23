/*
 * A136690  Final nonzero digit of n! in base 3.
 * https://oeis.org/A136690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = (n! / A060828(n)) mod 3 Recurrence (valid for all n ≥ 0):
 *            a(0) = 1, a(1) = 1 a(6*n) = a(6*n+1) = a(2*n) a(6*n+2) = 3 -
 *            a(2*n) a(6*n+3) = a(6*n+4) = 3 - a(2*n+1) a(6*n+5) = a(2*n+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0 || n == 1)
        return 1;

    idx_t k = n / 6;
    idx_t r = n % 6;
    seq_t val;

    switch (r) {
    case 0:                 /* a(6k)   = a(2k)   */
    case 1:                 /* a(6k+1) = a(2k)   */
        return a(2 * k);
    case 2:                 /* a(6k+2) = 3 - a(2k)   */
        val = a(2 * k);
        return 3 - val;
    case 3:                 /* a(6k+3) = 3 - a(2k+1) */
    case 4:                 /* a(6k+4) = 3 - a(2k+1) */
        val = a(2 * k + 1);
        return 3 - val;
    case 5:                 /* a(6k+5) = a(2k+1)   */
        return a(2 * k + 1);
    default:
        /* unreachable */
        return 1;
    }
}
