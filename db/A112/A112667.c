/*
 * A112667  a(n+1) is the sum of the units digit of a(n) and the square of the
 *          tens digit of a(n).
 * https://oeis.org/A112667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=97; for n≥1, a(n+1)= (a(n)%10)+((a(n)/10)%10)^2; It can be
 *            shown that a(n)=1 for all n>6
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses only integer arithmetic and no lookup tables. */
seq_t a(idx_t n)
{
    /* For n>6 the sequence is constant 1. */
    if (n > 6)
        return 1;

    seq_t val = 97;                   /* a(1) */
    if (n == 1)
        return val;

    /* Compute iteratively up to the desired index. */
    for (idx_t i = 1; i < n; ++i) {
        seq_t u = val % 10;              /* units digit */
        seq_t t = (val / 10) % 10;       /* tens digit */
        val = u + t * t;                 /* next term */
    }
    return val;
}
