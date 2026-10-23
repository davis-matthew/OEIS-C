/*
 * A032834  Numbers with digits 3 and 4 only.
 * https://oeis.org/A032834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(2*n+1) = 10*a(n)+3, a(2*n+2) = 10*a(n)+4 with a(1)=3, a(2)=4
 * Range:     u8 n <= 6, u16 n <= 62, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset is 1, return 0 for n==0 (undefined position) */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)4;

    /* Recursively compute the term.  Each step reduces n close to half,
       so the recursion depth is O(log n). */
    if (n & 1U) { /* odd: n = 2*k+1 */
        return (seq_t)10 * a(n / 2) + (seq_t)3;
    } else {      /* even: n = 2*k+2  =>  k = n/2 - 1 */
        return (seq_t)10 * a(n / 2 - 1) + (seq_t)4;
    }
}
