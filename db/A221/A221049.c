/*
 * A221049  Expansion of (1+2*x+3*x^2-x^3)/((1-x)*(1+x)*(1-2*x)*(1+2*x)).
 * https://oeis.org/A221049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A221049: Expansion of (1+2*x+3*x^2-x^3)/((1-x)*(1+x)*(1-2*x)*(1+2*x)).
 * Formula: a(0)=1, a(1)=2.
 * For n>1:
 *   if n is even:   a(n) = a(n-1) * 4
 *   if n is odd:    a(n) = a(n-1) + 1
 */
seq_t a(idx_t n)
{
    /* Handle the trivial cases first */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    seq_t val = (seq_t)2;          /* This holds a(n-1); start at a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        if ((i & 1ULL) == 0) {      /* Even index */
            val = val * (seq_t)4;
        } else {                    /* Odd index */
            val = val + (seq_t)1;
        }
    }
    return val;
}
