/*
 * A066446  Number of unordered divisor pairs of n.
 * https://oeis.org/A066446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = C(d(n), 2) = d(n)*(d(n)-1)/2 where d(n) is the number of
 *            positive divisors of n
 * Range:     u8 n <= 359, u16 n <= 9029, u32 n <= 9029, u64 n <= 9029,
 *            u128 n <= 9029, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of divisors d(n) for n>=1.
   Uses trial division up to sqrt(n). */
static seq_t divisor_count(idx_t n)
{
    if (n <= 1) return 1;              /* d(1)=1 */

    seq_t result = 1;
    idx_t temp = n;

    /* Count factor 2 */
    idx_t cnt = 0;
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        ++cnt;
    }
    if (cnt) result *= (seq_t)(cnt + 1);

    /* Count odd factors */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            result *= (seq_t)(cnt + 1);
        }
    }
    /* Remaining prime factor >1 */
    if (temp > 1) result *= 2;

    return result;
}

seq_t a(idx_t n)
{
    seq_t d = divisor_count(n);
    /* Compute d*(d-1)/2 in seq_t arithmetic */
    return d * (d - 1) / 2;
}
