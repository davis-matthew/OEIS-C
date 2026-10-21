/*
 * A092476  Numbers that are congruent to {1, 3, 9} mod 13.
 * https://oeis.org/A092476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (piecewise)
   a(3k-2) = 13k-12,
   a(3k-1) = 13k-10,
   a(3k)   = 13k-4
   ( works for n ≥ 1 )
*/
seq_t a(idx_t n)
{
    if (n == 0)              /* undefined for n<1; harmless fallback */
        return (seq_t)0;

    idx_t r = n % 3;          /* 0,1,2  ->  3k,3k-2,3k-1 respectively */
    idx_t k;

    if (r == 0) {             /* n = 3k */
        k = n / 3;
        return ((seq_t)13 * (seq_t)k) - (seq_t)4;
    } else if (r == 1) {      /* n = 3k-2  =>  k = (n+2)/3 */
        k = (n + 2) / 3;
        return ((seq_t)13 * (seq_t)k) - (seq_t)12;
    } else {                  /* r == 2, n = 3k-1 => k = (n+1)/3 */
        k = (n + 1) / 3;
        return ((seq_t)13 * (seq_t)k) - (seq_t)10;
    }
}
