/*
 * A159754  Numbers n with property that n^2 ends with 81.
 * https://oeis.org/A159754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 2621, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A159754: numbers n such that n^2 ends with 81.
   Formula: a(1) = 9, a(2) = 41,
            a(n) = a(n-2) + 50   for n ≥ 3      (OEIS)
*/
seq_t a(idx_t n) {
    /* The sequence is defined for n ≥ 1.  We guard against n=0
       to avoid unsigned underflow in the expressions for n-1 and n-2. */
    if (n == 0) return 0;          /* offset is 1; this case is unused */
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)41;

    /* For n ≥ 3.  The parity of n determines which base value to use. */
    if (n & 1U) { /* odd n */
        return (seq_t)9 + (seq_t)50 * ((seq_t)((n - 1) / 2));
    } else {     /* even n */
        return (seq_t)41 + (seq_t)50 * ((seq_t)((n - 2) / 2));
    }
}
