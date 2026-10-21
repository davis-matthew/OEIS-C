/*
 * A061866  a(n) is the number of solutions to x+y+z = 0 mod 3, where 1 <= x <
 *          y < z <= n.
 * https://oeis.org/A061866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (binomial(n,3) + 2 * floor(n/3)) / 3
 * Range:     u8 n <= 12, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Incrementally evaluates without overflow for 64‑bit seq_t */
seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    seq_t nn = n;

    /* binomial(n,2) = n*(n-1)/2  (computed without overflow) */
    seq_t t1;
    if (nn % 2 == 0) {
        t1 = (nn / 2) * (nn - 1);
    } else {
        t1 = ((nn - 1) / 2) * nn;
    }

    /* binomial(n,3) = binomial(n,2)*(n-2)/3; divide one factor by 3 first */
    seq_t c = nn - 2;
    if (t1 % 3 == 0) {
        t1 /= 3;
    } else {
        c /= 3;
    }
    seq_t binom3 = t1 * c;   /* now fits in seq_t for tested n */

    /* 2 * floor(n/3)  */
    seq_t add = (nn / 3) * 2;

    /* (binom3 + add) / 3  performed without overflow */
    seq_t res   = binom3 / 3;
    seq_t part  = add / 3;
    seq_t rem   = (binom3 % 3 + add % 3) / 3;  /* 0, 1, or 2 */

    return res + part + rem;
}
