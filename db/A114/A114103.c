/*
 * A114103  n multiples of n such that a(n) is a multiple of n. The n-th group
 *          contains n multiples of n. Arranged sequentially the n-th term is
 *          a multiple of n.
 * https://oeis.org/A114103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, look
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 1285, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A114103: a(n) = lcm(g, (g-1)*g/2 + k)
   where g is the smallest integer with g*(g+1)/2 ≥ n,
   and k = n - (g-1)*g/2.  Group g contains g terms.  */

/* Euclidean algorithm for seq_t type. */
static inline seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != (seq_t)0)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;            /* undefined for n < 1 */

    /* Find group index g such that g*(g+1)/2 ≥ n. */
    idx_t g = 1;
    while (((g + 1) * g) / 2 < n)
        ++g;

    idx_t prev = (g - 1) * g / 2;   /* number of terms before group g */
    idx_t k = n - prev;             /* position within the group (1‑based) */

    seq_t gseq = (seq_t)g;
    seq_t base = ((seq_t)(g - 1) * gseq) / 2;   /* (g-1)*g/2  */
    seq_t val  = base + (seq_t)k;               /* base + k   */

    seq_t d = gcd_seq(gseq, val);
    return (gseq / d) * val;                     /* lcm(g, val) */
}
