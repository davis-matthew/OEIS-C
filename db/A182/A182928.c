/*
 * A182928  Triangular array read by rows: [T(n,k),k=1..tau(n)] =
 *          [-n!/(d*(-(n/d)!)^d), d|n].
 * https://oeis.org/A182928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf
 * Formula:   a(n) = the nth element of the triangular array read by rows. For
 *            each row m (starting at 1) the entries are taken over all
 *            divisors d|m in ascending order, with value T(m,d) = -m! / ( d *
 *            ( -(m/d)! )^d ). The sign of the result is positive when d is
 *            odd and negative when d is even
 * Range:     i8 n <= 11, i16 n <= 17, i32 n <= 36, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: compute factorial of an integer (exact integer arithmetic). */
static seq_t factorial_of(idx_t m)
{
    seq_t r = 1;
    for (idx_t i = 2; i <= m; ++i) {
        r *= (seq_t)i;
    }
    return r;
}

/* Helper: compute integer power of a seq_t base by an integer exponent. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i) {
        result *= base;
    }
    return result;
}

/* Public function: returns the nth term of the sequence. */
seq_t a(idx_t n)
{
    /* No negative offsets are meaningful; the sequence is defined for n>=1. */
    idx_t count = 0;

    for (idx_t row = 1; ; ++row) {
        seq_t fac_row = factorial_of(row);

        for (idx_t d = 1; d <= row; ++d) {
            if (row % d != 0) continue;           /* d is a divisor of row */
            ++count;
            if (count == n) {
                idx_t m_over_d = row / d;
                seq_t fac_md   = factorial_of(m_over_d);
                seq_t powfact  = pow_seq(fac_md, d);
                seq_t denom    = powfact * (seq_t)d;   /* d cast to seq_t */
                seq_t val      = fac_row / denom;
                if ((d & 1) == 0) val = -val;         /* sign: even d -> negative */
                return val;
            }
        }
    }
}
