/*
 * A201371  Number of n X 4 0..1 arrays with rows and columns
 *          lexicographically nondecreasing read forwards, and nonincreasing
 *          read backwards.
 * https://oeis.org/A201371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + 4*n^3 - 13*n^2 + 32*n) / 12
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the closed‑form expression for sequence A201371. */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; return 0 for n == 0 to avoid
       undefined behaviour when called outside the offset. */
    if (n == 0)
        return 0;

    seq_t N = (seq_t)n;

    /* Compute the numerator n^4 + 4n^3 - 13n^2 + 32n.
       All arithmetic is performed in the (potentially large) type
       seq_t, so there is no risk of overflow for the expected range
       of n (up to about 10,000). */
    seq_t term = N * N * N * N;                     /* n^4          */
    term += (seq_t)4 * N * N * N;                   /* +4n^3        */
    term -= (seq_t)13 * N * N;                      /* -13n^2       */
    term += (seq_t)32 * N;                          /* +32n         */

    /* Divide by 12 (the numerator is always divisible by 12). */
    return term / (seq_t)12;
}
