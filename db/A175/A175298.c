/*
 * A175298  Smallest number >=n whose binary representation is palindromic and
 *          has a 1 whenever the binary representation of n has a 1.
 * https://oeis.org/A175298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n OR rev_bits(n) (reverse bits of n using the same bit
 *            length)
 * Range:     u8 n <= 255, u16 n <= 8652, u32 n <= 8652, u64 n <= 8652,
 *            u128 n <= 8652, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the smallest number ≥ n that is binary‑palindromic and has all 1‑bits of n. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t res = (seq_t)n;          /* start with n itself                      */
    idx_t len = 0;                 /* bit length of n (n>0)                    */
    idx_t t   = n;
    while (t) {                    /* count bits until t becomes zero         */
        ++len;
        t >>= 1;
    }

    for (idx_t i = 0; i < len; ++i) /* force symmetry with the reverse          */
    {
        if (((res >> i) & 1) != 0)
            res |= ((seq_t)1 << (len - 1 - i));
    }
    return res;
}
