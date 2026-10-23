/*
 * A120158  a(n) = 14 + floor((1 + Sum_{j=1..n-1} a(j))/3).
 * https://oeis.org/A120158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14 + floor((1 + Sum_{j=1}^{n-1} a(j)) / 3)
 * Range:     u8 n <= 11, u16 n <= 30, u32 n <= 68, u64 n <= 146,
 *            u128 n <= 300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for n >= 1.  For n = 0 the behaviour is unspecified
   and simply returns 0.  The implementation keeps the running
   sum as (sum / 3) and (sum mod 3) separately, avoiding overflow
   of the intermediate sum even when a(n) itself fits in seq_t. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* behaviour below offset is irrelevant */

    seq_t val = (seq_t)14;       /* a(1) */
    if (n == 1)
        return val;

    /* Maintain sum = sum_{j=1}^{i-1} a(j) as (div3, rem) where div3 = sum/3,
       rem   = sum%3.  Initially sum = a(1) = 14. */
    seq_t div3 = val / (seq_t)3;      /* 4  for 14 */
    unsigned int rem = (unsigned int)(val % (seq_t)3); /* 2  for 14 */

    /* Iterate from i = 2 to n */
    for (idx_t i = 2; i <= n; ++i) {
        /* Compute a(i) using the formula.
           floor((S+1)/3) = div3 + (rem==2 ? 1 : 0) */
        unsigned int extra = (rem == 2) ? 1 : 0;
        val = (seq_t)14 + div3 + (seq_t)extra;

        /* Update (div3, rem) to reflect sum += val */
        seq_t a_div3 = val / (seq_t)3;
        unsigned int a_rem = (unsigned int)(val % (seq_t)3);

        unsigned int new_rem = rem + a_rem;   /* 0 <= new_rem <= 4 */
        unsigned int carry = new_rem / 3;     /* 0 or 1 */
        rem = new_rem % 3;

        div3 += a_div3 + (seq_t)carry;
    }

    return val;
}
