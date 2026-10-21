/*
 * A081049  a(n) = n! - Stirling1(n+1, 2) where Stirling1 is A048994.
 * https://oeis.org/A081049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 19, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n! - Stirling1(n+1, 2)
 * (Stirling1 is the signed Stirling number of the first kind)
 *
 * The function computes both n! and the required Stirling number by a single
 * forward recurrence.  Only O(1) storage is needed and no dynamic memory
 * allocation is performed.  It uses only arithmetic operations allowed
 * on seq_t and idx_t, so it is valid in both C11 and C++17 modes.
 */
seq_t a(idx_t n)
{
    seq_t fact = 1;        /* n!   (fact(0) = 0! = 1)      */
    seq_t s1   = 1;        /* s(1,1) = 1                    */
    seq_t s2   = 0;        /* s(1,2) = 0                    */

    for (idx_t i = 1; i <= n; ++i) {
        /* factorial */
        fact = fact * (seq_t)i;

        /* Stirling recurrence:
         *   s(n+1,1) = -n * s(n,1)
         *   s(n+1,2) = s(n,1) - n * s(n,2)
         */
        seq_t new_s2 = s1 - (seq_t)i * s2;
        seq_t new_s1 = -(seq_t)i * s1;

        s2 = new_s2;
        s1 = new_s1;
    }

    return fact - s2;
}
