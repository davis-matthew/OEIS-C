/*
 * A239569  Number of ways to place 3 points on a triangular grid of side n so
 *          that no two of them are adjacent.
 * https://oeis.org/A239569
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(n-2)*(n^4+6*n^3-23*n^2-92*n+264)/48 (offset 2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 75, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 2 the value is undefined in the sequence, return 0. */
    if (n < 2) return 0;

    /* Use 64‑bit signed arithmetic for the polynomial part, which fits
       safely for all n up to 10 000. */
    long long Nll = (long long)n;
    long long poly = Nll * Nll * Nll * Nll          /* n^4          */
                    + 6LL * Nll * Nll * Nll          /* +6 n^3       */
                    - 23LL * Nll * Nll              /* -23 n^2      */
                    - 92LL * Nll                    /* -92 n        */
                    + 264LL;                        /* +264         */

    seq_t N      = (seq_t)n;      /* convert index to seq_t for later ops  */
    seq_t n1     = N - 1;           /* (n-1)              */
    seq_t n2     = N - 2;           /* (n-2)              */
    seq_t poly_s = (seq_t)poly;     /* polynomial term    */

    /* Compute the product in seq_t form; the intermediate product fits
       comfortably in seq_t's range (for the arbitrary‑precision build
       it is abundant). */
    seq_t num = n1 * n2 * poly_s;

    /* Division by 48 is exact by the formula. */
    return num / (seq_t)48;
}
