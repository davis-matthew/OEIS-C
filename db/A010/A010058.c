/*
 * A010058  1 if n is a Catalan number else 0.
 * https://oeis.org/A010058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a Catalan number, otherwise 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*  n = 0 is never a Catalan number */
    if (n == 0) {
        return (seq_t)0;
    }

    seq_t target = (seq_t)n;          /* value to test */

    /* C_0 = 1 */
    seq_t c = (seq_t)1;
    if (c == target) {
        return (seq_t)1;              /* n = 1 */
    }

    /* Compute successive Catalan numbers C_k using the recurrence
       C_k = C_{k-1} * (4k - 2) / (k + 1)  for k >= 1.   */
    for (idx_t k = 1; ; ++k) {
        /* numerator = C_{k-1} * (4k - 2) */
        seq_t mult   = ((seq_t)k * (seq_t)4) - (seq_t)2;   /* 4k-2 */
        seq_t denom  = (seq_t)(k + (idx_t)1);               /* k+1 */
        seq_t next   = c * mult / denom;                    /* exact division */

        if (next == target) {
            return (seq_t)1;          /* match found */
        }
        if (next > target) {
            return (seq_t)0;          /* surpass target, not a Catalan number */
        }
        c = next;                     /* prepare for next iteration */
    }
}
