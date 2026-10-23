/*
 * A166799  Number of nX2 1..6 arrays containing at least one of each value,
 *          all equal values connected, rows considered as a single number in
 *          nondecreasing order, and columns considered as a single number in
 *          nondecreasing order.
 * https://oeis.org/A166799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/226800)*(n-1)*(n-2)*(n^8 + 53*n^7 + 1477*n^6 +
 *            23435*n^5 + 173779*n^4 + 177842*n^3 - 1867077*n^2 + 450450*n -
 *            113400)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)0;

    /* Use 128‑bit arithmetic for intermediate products.
       It is valid for all builds: for 64‑bit seq_t the cast
       to __int128 is safe, and for larger seq_t it simply
       promotes the arithmetic.    */
    __int128 tn = (unsigned long long)n;   /* cast to 128‑bit */

    /* Compute the polynomial via Horner's method.     */
    __int128 p = ( __int128)1;              /* constant term for Horner */
    p = p * tn + 53;
    p = p * tn + 1477;
    p = p * tn + 23435;
    p = p * tn + 173779;
    p = p * tn + 177842;
    p = p * tn - 1867077;
    p = p * tn + 450450;
    p = p * tn - 113400;

    /* Compute (n-1)*(n-2) */
    __int128 fac = (tn - 1) * (tn - 2);

    /* Final division */
    __int128 result = fac * p / 226800;

    return (seq_t)result;
}
