/*
 * A078832  Smallest prime contained as binary substring in binary
 *          representation of n>1, a(1)=1.
 * https://oeis.org/A078832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A078832 – Smallest prime in the binary representation of n (for n>1),
    with a(1)=1.
    The binary string of a prime p appears as a contiguous substring of
    n's binary representation.  For n>1 the only primes that can appear
    are 2 ("10") and 3 ("11").  The value is 2 except when n is of the
    form 2^k-1 (an all‑ones number), in which case 2 is not present and
    the smallest prime is 3.  n==1 is a special case returning 1.
*/
seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;               /* special case */
    if ((n & (n + 1)) == 0) return (seq_t)3;   /* n = 2^k - 1 -> answer 3 */
    return (seq_t)2;                            /* otherwise answer 2 */
}
