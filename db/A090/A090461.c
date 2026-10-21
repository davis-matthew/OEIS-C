/*
 * A090461  Numbers k for which there exists a permutation of the numbers 1 to
 *          k such that the sum of adjacent numbers is a square.
 * https://oeis.org/A090461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth term of OEIS A090461.
   The sequence consists of all integers k for which there is a
   permutation of 1..k with adjacent sums equal to a perfect square.
   Main theorem: every integer k ≥ 25 satisfies the property.
   Hence the sequence is:
   a(1)=15, a(2)=16, a(3)=17, a(4)=23,
   and for all n ≥ 5, a(n) = n + 20. */
seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly. */
    if (n <= 4) {
        static const seq_t first[4] = { (seq_t)15, (seq_t)16, (seq_t)17, (seq_t)23 };
        return first[n - 1];
    }
    /* For n ≥ 5 the value is simply n + 20. */
    return (seq_t)n + 20;
}
