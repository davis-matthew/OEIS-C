/*
 * A098794  a(n+1) = smallest number greater a(n) that ends in binary
 *          representation with floor(a(n)/4); a(1)=1.
 * https://oeis.org/A098794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A098794
 *  a(1) = 1
 *  a(n+1) = the smallest integer > a(n) that ends with the binary
 *           representation of floor(a(n)/4).  Equivalently if
 *           k = floor(a(n)/4) and len = bit length of k (1 if k==0),
 *           let M = 2^len; then
 *           a(n+1) = ((a(n) - k)/M + 1)*M + k
 *   (so a(n+1) ≡ k (mod M) and a(n+1) > a(n)).
 *
 *  This implementation computes a(n) by iterating from the
 *  first term and applying the recurrence; it works for any
 *  integer type seq_t that supports the usual arithmetic,
 *  comparison, and bitwise shift operators.
 */

static unsigned bitlen_seq_t(seq_t v)
{
    /* bit length of v; 0 → 1 (binary "0") */
    if (v == 0) return 1;
    unsigned len = 0;
    while (v)
    {
        v >>= 1;
        ++len;
    }
    return len;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n < 1, return 0 to avoid UB */
        return (seq_t)0;

    seq_t prev = (seq_t)1;               /* a(1) */
    if (n == 1)
        return prev;

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t k  = prev / (seq_t)4;              /* floor(prev/4) */
        unsigned len = bitlen_seq_t(k);         /* bit length of k (1 if k==0) */
        seq_t M = ((seq_t)1) << len;            /* 2^len */
        seq_t t = (prev - k) / M;               /* ((prev – k) / M) */
        prev   = (t + 1) * M + k;               /* next term */
    }
    return prev;
}
