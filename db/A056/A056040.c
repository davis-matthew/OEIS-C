/*
 * A056040  Swinging factorial, a(n) = 2^(n-(n mod 2))*Product_{k=1..n}
 *          k^((-1)^(k+1)).
 * https://oeis.org/A056040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! / (floor(n/2)!)^2 Equivalent: a(2m) = binomial(2m, m)
 *            a(2m+1) = (2m+1) * binomial(2m, m)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 30, u64 n <= 62, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case n = 0 (floor(0/2)=0) */
    if (n == 0)
        return (seq_t)1;

    idx_t m = n / 2;           /* floor(n/2)          */
    seq_t res = (seq_t)1;      /* will hold binomial(2m, m) */

    /* Compute binomial(2m, m) multiplicatively:
       res = product_{k=1..m} (m + k) / k   (exact division at each step) */
    for (idx_t k = 1; k <= m; ++k) {
        res = res * (seq_t)(m + k) / (seq_t)k;
    }

    if ((n & 1U) == 0)          /* n even: a(n) = binomial(2m, m) */
        return res;
    else                        /* n odd : a(n) = (2m+1) * binomial(2m, m) */
        return res * (seq_t)n;
}
