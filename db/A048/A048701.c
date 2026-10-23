/*
 * A048701  List of binary palindromes of even length (written in base 10).
 * https://oeis.org/A048701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (n << (floor_log_2(n)+1)) + reverse_bits(n)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the base‑10 integer obtained by mirroring the binary
   representation of n to an even‑length palindrome. */

seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;                /* a(0) = 0 */
    /* Compute floor(log2(n)) */
    unsigned m = 0;
    idx_t tmp = n;
    while (tmp > 1) {
        tmp >>= 1;
        ++m;
    }
    unsigned len = m + 1;                              /* bit length of n */
    /* Reverse the bits of n within len bits */
    seq_t rev = 0;
    for (unsigned i = 0; i <= m; ++i) {
        if ((n >> i) & (idx_t)1) {
            rev |= ((seq_t)1) << (len - 1 - i);
        }
    }
    /* Construct the even‑length binary palindrome */
    return ((seq_t)n << len) + rev;
}
