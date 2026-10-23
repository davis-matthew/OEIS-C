/*
 * A101328  Recurring numbers in the count of consecutive composite numbers
 *          between balanced primes and their lower or upper prime neighbors.
 * https://oeis.org/A101328
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*n - 13 for n ≥ 3, a(2) = 1, a(n) undefined for n < 2
 *            (This follows from the definition a(n) =
 *            nextprime(A054342(n)+1)-A054342(n)-1 which results in an
 *            arithmetic progression with difference 6 after the first term.)
 * Range:     u8 n <= 44, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                       /* undefined for offset 2; behaviour not required */

    if (n == 2)
        return 1;                       /* first term of the sequence */

    /* For n ≥ 3: a(n) = 6*(n-3) + 5  which is equivalent to 6*n - 13 */
    return (seq_t)6 * (seq_t)(n - 3) + 5;
}
