/*
 * A165789  a(n) is the smallest positive integer k that when written in
 *          binary, and leading 0's of k are ignored, contains the reversal of
 *          the digits of binary n.
 * https://oeis.org/A165789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A165789: a(n) is the minimal positive integer k whose binary
 * representation contains the reverse of the binary digits of n.
 * For odd n (n LSB is 1) the pattern itself has leading 1, so the
 * smallest k is just the binary reversal of n:
 *
 *     a(n) = rev_bits(n)            (n odd)
 *
 * For even n the reversed pattern starts with 0, so we must prepend a
 * leading 1.  Let L be the bit‑length of n.  Then
 *
 *     a(n) = (1 << L) + rev_bits(n)  (n even)
 *
 * The implementation below follows this logic directly.
 */

#include <stdint.h>     /* for size_t, if needed */

static inline seq_t reverse_bits(idx_t n)
{
    seq_t rev = 0;
    idx_t  t   = n;
    /* Count bits (L) in parallel while building the reversed value. */
    while (t) {
        rev = (rev << 1) | (seq_t)(t & 1);
        t >>= 1;
    }
    return rev;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used, but safe fallback */

    seq_t rev = 0;
    idx_t t   = n;
    idx_t L   = 0;                 /* bit‑length of n */

    /* Build rev and count bits. */
    while (t) {
        rev = (rev << 1) | (seq_t)(t & 1);
        t >>= 1;
        ++L;
    }

    /* If n is odd, pattern starts with 1 – no leading bit needed. */
    if (n & 1) {
        return rev;
    }

    /* Even n: prepend the necessary leading 1 bit. */
    return ((seq_t)1 << L) + rev;
}
