/*
 * A164765  Partial sums of [A080782^2].
 * https://oeis.org/A164765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n(n+1)(2n+1)/6, plus (2n+1) when (n+1) % 3 == 0
 *            (equivalent to the given sin^2 expression)
 * Range:     u8 n <= 8, u16 n <= 57, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the integer type used for the sequence. */
    seq_t nseq = (seq_t)n;

    /* Prepare the factors of the sum‑of‑squares numerator:
       n, n+1, and 2n+1. */
    seq_t t1 = nseq;                     /* n          */
    seq_t t2 = nseq + (seq_t)1;          /* n+1        */
    seq_t t3 = nseq * (seq_t)2 + (seq_t)1; /* 2n+1      */

    /* Divide by 2 once, by 3 once, to keep the intermediate product
       within the limits of the 64‑bit build while still working for
       arbitrary‑precision.  The divisions are safe because the total
       product is guaranteed to be divisible by 6. */
    if ( (t1 % (seq_t)2) == (seq_t)0 )
        t1 = t1 / (seq_t)2;
    else if ( (t2 % (seq_t)2) == (seq_t)0 )
        t2 = t2 / (seq_t)2;
    else
        t3 = t3 / (seq_t)2;

    if ( (t1 % (seq_t)3) == (seq_t)0 )
        t1 = t1 / (seq_t)3;
    else if ( (t2 % (seq_t)3) == (seq_t)0 )
        t2 = t2 / (seq_t)3;
    else
        t3 = t3 / (seq_t)3;

    /* Compute the sum of squares. */
    seq_t sumsq = t1 * t2 * t3;

    /* Term (2n+1) for the special case. */
    seq_t term2 = nseq * (seq_t)2 + (seq_t)1;

    /* Add the extra term only when (n+1) is a multiple of 3. */
    if ( ((n + (idx_t)1) % (idx_t)3) == (idx_t)0 )
        return sumsq + term2;
    else
        return sumsq;
}
