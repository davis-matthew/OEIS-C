/*
 * A099767  Number of n-digit palindromes in base n.
 * https://oeis.org/A099767
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = (n-1) * n^( floor((n+1)/2) - 1 ) , for n >= 2
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 26, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the OEIS sequence A099767: number of n‑digit palindromes in base n */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 2.  For n < 2 the value is irrelevant. */
    if (n < 2) {
        return 0;
    }

    /* exponent = floor((n+1)/2) - 1  (the power of n) */
    idx_t exp = (n + 1) / 2 - 1;   /* integer arithmetic on idx_t */

    /* binary exponentiation: compute n^exp using seq_t arithmetic */
    seq_t base = (seq_t)n;
    seq_t power = 1;                /* initializes as seq_t */

    while (exp) {
        if (exp & 1) {
            power *= base;
        }
        base *= base;
        exp >>= 1;
    }

    /* result = (n-1) * power  */
    seq_t result = (seq_t)(n - 1) * power;
    return result;
}
