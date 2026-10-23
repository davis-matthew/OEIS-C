/*
 * A027484  a(n) = n*(n^4-1)/2.
 * https://oeis.org/A027484
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^4-1)/2
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 97, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 2  (a(2) is the first term) */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 2.  The function ignores lower
       indices; behaviour below offset is unspecified. */
    if (n < 2) return 0;

    seq_t nn = (seq_t)n;          /* cast index to seq_t for arithmetic */

    seq_t n2 = nn * nn;           /* n^2           */
    seq_t n4 = n2 * n2;           /* n^4           */

    /* Compute n*(n^4-1)/2 without overflow:
       If n is even:  a = (n/2) * (n^4-1)
       If n is odd :  a = n * ((n^4-1)/2)   */
    if ((nn & 1) == 0) {          /* n even */
        return (nn >> 1) * (n4 - 1);
    } else {                      /* n odd  */
        return nn * ((n4 - 1) >> 1);
    }
}
