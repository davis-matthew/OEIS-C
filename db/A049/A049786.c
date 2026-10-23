/*
 * A049786  a(n) = T(n,n-1), array T as in A049783.
 * https://oeis.org/A049786
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..n-2} (n-1 mod floor((n-2)/j))
 * Range:     u8 n <= 179, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the definition from OEIS A049786 */

seq_t a(idx_t n)
{
    if (n < 2) {
        /* Undefined for n < 2; return 0 as a safe default. */
        return (seq_t)0;
    }

    idx_t m = n - 1;          /* n-1 used in the modulus */
    idx_t limit = n - 2;      /* upper bound for j */

    seq_t sum = (seq_t)0;
    for (idx_t j = (idx_t)1; j <= limit; ++j) {
        idx_t divisor = limit / j;     /* floor((n-2)/j) */
        seq_t rem = ((seq_t)m) % (seq_t)divisor;
        sum += rem;
    }
    return sum;
}
