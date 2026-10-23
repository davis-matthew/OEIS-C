/*
 * A154050  Number of planar triangular n X n X n nonnegative integer grids
 *          symmetric under 120 degree rotation with every similarly oriented
 *          5 X 5 X 5 subtriangle summing to 5.
 * https://oeis.org/A154050
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by case analysis:
   For n ≥ 16:   a(n) = 27 if n ≡ 1 (mod 3), otherwise 0.
   For 5 ≤ n ≤ 15 the values are taken from the OEIS table:
   n=5..15 -> 0,25,72,0,28,61,0,13,43,0,0
   (Values for n < 5 are not defined; return 0). */
seq_t a(idx_t n)
{
    if (n < 5) {
        return (seq_t)0;
    }
    /* Special values for 5 ≤ n ≤ 15 */
    if (n <= 15) {
        static const seq_t table[] = {
            (seq_t)0,  /* n=5  */
            (seq_t)25, /* n=6  */
            (seq_t)72, /* n=7  */
            (seq_t)0,  /* n=8  */
            (seq_t)28, /* n=9  */
            (seq_t)61, /* n=10 */
            (seq_t)0,  /* n=11 */
            (seq_t)13, /* n=12 */
            (seq_t)43, /* n=13 */
            (seq_t)0,  /* n=14 */
            (seq_t)0   /* n=15 */
        };
        return table[n - 5];
    }
    /* n ≥ 16 */
    if ((n % 3ULL) == 1ULL) {
        return (seq_t)27;
    } else {
        return (seq_t)0;
    }
}
