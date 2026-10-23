/*
 * A027604  a(n) = n^3 + (n+1)^3 + (n+2)^3 + (n+3)^3 + (n+4)^3.
 * https://oeis.org/A027604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n^3 + 30*n^2 + 90*n + 100
 * Range:     u8 n <= 1, u16 n <= 21, u32 n <= 948, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the definition a(n) = sum_{k=0..4} (n+k)^3 directly. */
seq_t a(idx_t n)
{
    /* Convert index to the appropriate arithmetic type */
    seq_t sn = (seq_t)n;

    /* Compute powers of n using a sequence of multiplications.
       All operations stay within seq_t as required. */
    seq_t n2 = sn * sn;          /* n^2 */
    seq_t n3 = n2 * sn;          /* n^3 */

    /* Apply the explicit polynomial formula. */
    seq_t res = 5 * n3 + 30 * n2 + 90 * sn + 100;
    return res;
}
