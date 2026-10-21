/*
 * A063205  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          29 ).
 * https://oeis.org/A063205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 2, a(2) = 7. For n >= 3, a(n) = 11 + 14*floor((n-3)/3) +
 *            [0,6,10][(n-3)%3]
 * Range:     u8 n <= 55, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows from the first two differences 5,4 and the repeating
   pattern of differences 6,4,4 for n >= 3.  */
seq_t a(idx_t n)
{
    /* Basic cases */
    if (n == 1) return 2;
    if (n == 2) return 7;

    /* For n >= 3 */
    idx_t t   = n - 3;            /* number of pattern differences to add  */
    idx_t full = t / 3;           /* complete cycles of {6,4,4}            */
    idx_t rem  = t % 3;           /* partial cycle length                 */

    /* Partial sums of the pattern 6,4,4: {0,6,10} */
    static const seq_t partial[3] = {0, 6, 10};

    seq_t res = 11;                        /* base value a(3) */
    res += (seq_t)14 * (seq_t)full;        /* add complete cycles */
    res += partial[rem];                   /* add partial cycle */

    return res;
}
