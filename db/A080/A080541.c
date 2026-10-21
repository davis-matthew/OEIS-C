/*
 * A080541  In binary representation: keep the first digit and left-rotate the
 *          others.
 * https://oeis.org/A080541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = keep the most significant bit of n and left‑rotate all
 *            lower bits by one. That is, for n>1, let len=⌊log₂n⌋+1,
 *            msb=1<<(len‑1), rest=n^msb. Then a(n)=msb | (
 *            ((rest<<1)|(rest>>(len-2))) & ( (1<<(len-1))-1 ) ). a(1)=1
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;               /* a(1)=1, a(0) unused */
    seq_t val = (seq_t)n;
    /* compute bit length of val */
    unsigned len = 0;
    seq_t t = val;
    while (t) { len++; t >>= 1; }

    seq_t msb = (seq_t)1 << (len - 1);          /* most significant bit */
    seq_t rest = val ^ msb;                    /* lower bits */

    if (len == 1) return msb; /* only the msb exists */

    unsigned r = len - 1; /* number of lower bits */
    seq_t mask = ((seq_t)1 << r) - 1;
    /* left‑rotate the lower bits by one */
    rest = ((rest << 1) | (rest >> (r - 1))) & mask;

    return msb | rest;
}
