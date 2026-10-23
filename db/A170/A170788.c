/*
 * A170788  a(n) = n^9*(n^6 + 1)/2.
 * https://oeis.org/A170788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^9*(n^6 + 1)/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 20, u128 n <= 388,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Promote index to the sequence type once */
    seq_t x = (seq_t)n;

    /* Compute successive powers of x avoiding multiplications that might
       overflow before the division by 2 is performed.  The intermediate
       values are kept small enough for the % 64‑bit build for all n
       whose results fit into 64 bits; for big‑integer builds the
       operator overloads handle arbitrary size. */
    seq_t x2 = x * x;          /* x^2   */
    seq_t x3 = x2 * x;         /* x^3   */
    seq_t x4 = x2 * x2;        /* x^4   */
    seq_t x5 = x4 * x;         /* x^5   */
    seq_t x6 = x3 * x3;        /* x^6   */
    seq_t x7 = x6 * x;         /* x^7   */
    seq_t x8 = x4 * x4;        /* x^8   */
    seq_t x9 = x8 * x;         /* x^9   */

    /* Apply the division by 2 to the even factor first to keep the
       products small. */
    seq_t result;
    if ((x & (seq_t)1) == 0) {           /* x even → x^9 is even */
        seq_t half = x9 / (seq_t)2;
        result = half * (x6 + (seq_t)1);
    } else {                              /* x odd → x^6 + 1 is even */
        seq_t half = (x6 + (seq_t)1) / (seq_t)2;
        result = x9 * half;
    }
    return result;
}
