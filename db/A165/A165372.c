/*
 * A165372  Number of slanted n X 3 (i=1..n) X (j=i..3+i-1) 1..4 arrays with
 *          all 1s connected, all 2s connected, all 3s connected, all 4s
 *          connected, 1 in the upper left corner, 2 in the upper right
 *          corner, 3 in the lower left corner, and 4 in the lower right
 *          corner.
 * https://oeis.org/A165372
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A165372
 * Empirical/formula:    a(2)=9,  a(n)=8*(8*n-19)  for n>2
 * Recurrence:           a(n)=2*a(n-1)-a(n-2)   for n≥5  (not used in code)
 * Offset:                2  (a(2) is the first term)
 *
 * This implementation returns the sequence value for any n≥2.
 * For n<2 the function returns 0 (undefined in the sequence).
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                       /* undefined block, return 0 */
    if (n == 2)                          /* special case, offset value */
        return 9;
    seq_t nn = (seq_t)n;                 /* promote to seq_t to avoid overflow */
    return 8 * ((8 * nn) - 19);          /* concise formula valid for n>2 */
}
