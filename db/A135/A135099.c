/*
 * A135099  a(1)=1, a(n) = a(n-1) + n^5 if n odd, a(n) = a(n-1) + n^3 if n is
 *          even.
 * https://oeis.org/A135099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 60, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; a(n)=a(n-1)+n^5 for n odd; a(n)=a(n-1)+n^3 for n even */
seq_t a(idx_t n)
{
    /* a(1) is the first term */
    seq_t sum = 1;
    if (n <= 1)
        return sum;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t k    = (seq_t)i;          /* convert index to seq_t to avoid truncation */
        seq_t k_sq = k * k;             /* k^2 */

        if (i & 1U) {                   /* n is odd: add n^5 */
            seq_t k4 = k_sq * k_sq;     /* k^4 */
            seq_t k5 = k4 * k;          /* k^5 */
            sum += k5;
        } else {                        /* n is even: add n^3 */
            seq_t k3 = k_sq * k;        /* k^3 */
            sum += k3;
        }
    }

    return sum;
}
