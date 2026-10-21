/*
 * A092095  a(n) = Sum_{i=0,1,2,...; n-k*i >= -n} |n-k*i| for k=4.
 * https://oeis.org/A092095
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=0}^{floor(n/2)} |n - 4*i| (k = 4)
 * Range:     u8 n <= 29, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived closed‑form: a(n) = n*(2*floor(n/4)+1 - floor(n/2)) */
/*                      + 2*floor(n/2)*(floor(n/2)+1) */
/*                      - 4*floor(n/4)*(floor(n/4)+1)   for n >= 5 */

seq_t a(idx_t n)
{
    /* step sizes */
    idx_t m = n / 2;   /* floor(n/2) */
    idx_t p = n / 4;   /* floor(n/4) */

    seq_t nm = (seq_t)n;          /* promote index to seq_t */
    seq_t mm = (seq_t)m;
    seq_t pp = (seq_t)p;

    seq_t result = nm * ( (2*pp + 1) - mm );          /* n*(2p+1 - m) */
    result += 2*mm*(mm + 1);                          /* + 2m(m+1) */
    result -= 4*pp*(pp + 1);                          /* - 4p(p+1) */

    return result;
}
