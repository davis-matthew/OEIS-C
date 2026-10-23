/*
 * A126269  Numbers k such that hcl(k,k) < hcl(k,k-1) where hcl(k,i) is the
 *          Huffman code length; see comments.
 * https://oeis.org/A126269
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is given by the recurrence * a(3)=3, a(4)=4, a(5)=9,
 *            a(6)=10, * a(n)=3*a(n-2) - 2*a(n-4) for n>6. * This definition
 *            yields the OEIS sequence A126269
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset of the sequence is 3: a(3) is the first term. */
    if (n < 3) return (seq_t)0;          /* undefined for n<3 */

    /* Handle the first four terms directly. */
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)4;
    if (n == 5) return (seq_t)9;
    if (n == 6) return (seq_t)10;

    /* Initialise a sliding window for indices i-4, i-3, i-2, i-1.  */
    seq_t a0 = (seq_t)3; /* a(3) */
    seq_t a1 = (seq_t)4; /* a(4) */
    seq_t a2 = (seq_t)9; /* a(5) */
    seq_t a3 = (seq_t)10;/* a(6) */
    seq_t curr = (seq_t)0; /* placeholder */

    /* Compute successive values up to n. */
    for (idx_t i = 7; i <= n; ++i) {
        /* a(i) = 3*a(i-2) - 2*a(i-4)  */
        curr = (3 * a2) - (2 * a0);

        /* Slide the window forward: drop a0, shift others left, add new curr */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = curr;
    }
    return curr;
}
