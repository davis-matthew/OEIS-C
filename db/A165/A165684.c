/*
 * A165684  Dimension of the space of Siegel cusp forms of genus 2 and
 *          dimension 2n (associated with full modular group Gamma_2).
 * https://oeis.org/A165684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^3 + 24*n^2 + 36*n + 400)/1080 * - (n mod 2)*n/24
 *            * - ((2*n^2 + 2*n) mod 3)*n/18)
 * Range:     u8 none, u16 n <= 33, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast to seq_t to avoid intermediate overflow */
    seq_t nn = (seq_t)n;
    /* main polynomial part */
    seq_t s = nn * nn * nn            /* n³          */
            + (seq_t)24 * nn * nn     /* 24 n²      */
            + (seq_t)36 * nn          /* 36 n       */
            + (seq_t)400;             /* constant   */

    /* correction involving n mod 2 */
    seq_t nmod2 = n & 1ull;                  /* 0 or 1   */
    seq_t t2    = (seq_t)45 * nmod2 * nn;    /* 45*n*(n%2) */

    /* correction involving (2*n² + 2*n) mod 3 */
    seq_t t3sc  = ( (seq_t)2 * nn * nn + (seq_t)2 * nn ) % (seq_t)3;
    seq_t t3    = (seq_t)60 * t3sc * nn;     /* 60*n*[(2n²+2n)%3] */

    /* combine and take floor division by 1080 */
    seq_t numer = s - t2 - t3;
    seq_t res   = numer / (seq_t)1080;

    return res;
}
