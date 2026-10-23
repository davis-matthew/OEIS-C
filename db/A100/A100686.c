/*
 * A100686  a(1) = 1, a(2) = 2; thereafter a(2n+1) = |a(2n)^2-a(2n-1)^2|,
 *          a(2n+2) = 2*a(2n-1)*a(2n).
 * https://oeis.org/A100686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=2; for n≥1, a(2n+1) = |a(2n)^2 - a(2n-1)^2|,
 *            a(2n+2) = 2*a(2n-1)*a(2n)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 10, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    /* array holds last three terms: arr[0]=a(k-3), arr[1]=a(k-2), arr[2]=a(k-1) */
    seq_t arr[3];
    arr[0] = (seq_t)1;          /* a(1) */
    arr[1] = (seq_t)2;          /* a(2) */
    /* a(3) derived from recurrence */
    arr[2] = (seq_t)3;          /* a(3) = |2^2-1^2| = 3 */
    if (n == 3) return arr[2];

    for (idx_t k = 4; k <= n; ++k)
    {
        seq_t new_term;
        if ((k & 1) == 0)           /* even: a(k) = 2*a(k-3)*a(k-2) */
        {
            new_term = ((seq_t)2 * arr[0]) * arr[1];
        }
        else                         /* odd: a(k) = |a(k-1)^2 - a(k-2)^2| */
        {
            seq_t sq1 = arr[2] * arr[2];
            seq_t sq2 = arr[1] * arr[1];
            new_term = (sq1 >= sq2) ? (sq1 - sq2) : (sq2 - sq1);
        }
        /* shift window: arr[0] <- arr[1], arr[1] <- arr[2], arr[2] <- new_term */
        arr[0] = arr[1];
        arr[1] = arr[2];
        arr[2] = new_term;
    }
    return arr[2];
}
