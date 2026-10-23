/*
 * A011917  [ n(n-1)(n-2)(n-3)/7 ].
 * https://oeis.org/A011917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)*(n-2)*(n-3)/7)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 328, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A011917: first few terms: 0,0,0,0,3,17,51,120,240,432,720,1131,... */

seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)0;

    /* Compute the product in seq_t to avoid overflow on the numerator.  */
    seq_t v = (seq_t)n;
    v *= (seq_t)(n - 1);
    v *= (seq_t)(n - 2);
    v *= (seq_t)(n - 3);
    v /= (seq_t)7;          /* integer division truncates toward zero (floor)   */
    return v;
}
