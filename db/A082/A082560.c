/*
 * A082560  a(1)=1, a(n)=2*a(n-1) if n is odd, or a(n)=a(n/2)+1 if n is even.
 * https://oeis.org/A082560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, look
 * Formula:   a(1)=1; a(n)=2*a(n-1) if n is odd, a(n)=a(n/2)+1 if n is even
 * Range:     u8 n <= 254, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n≥1; behaviour for n<1 is irrelevant. */
    if (n == 0) return 0;          /* safety for calls with n==0 */

    if (n == 1) return 1;          /* a(1)=1 */

    /* Recursively evaluate the definition. */
    if (n & 1)                     /* n is odd */
        return (seq_t)2 * a(n - 1); /* a(n)=2*a(n-1) */
    else
        return a(n / 2) + 1;       /* a(n)=a(n/2)+1 */
}
