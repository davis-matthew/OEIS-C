/*
 * A042763  Denominators of continued fraction convergents to sqrt(912).
 * https://oeis.org/A042763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0) = 1 a(1) = 5 For n ≥ 2: if n is even: a(n) = 60*a(n‑1) +
 *            a(n‑2) if n is odd : a(n) = 5*a(n‑1) + a(n‑2) This follows from
 *            the continued‑fraction expansion sqrt(912) = [30;(5,60)]
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)5;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev  = (seq_t)5;   /* a(1) */
    seq_t cur   = prev;       /* will hold a(i) */

    for (idx_t i = 2; i <= n; ++i) {
        if ((i & 1) == 0)          /* even i -> multiplier 60 */
            cur = prev * (seq_t)60 + prev2;
        else                       /* odd i  -> multiplier 5  */
            cur = prev * (seq_t)5 + prev2;
        prev2 = prev;
        prev  = cur;
    }

    return cur;
}
