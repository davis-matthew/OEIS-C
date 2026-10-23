/*
 * A178704  Partial sums of floor(3^n/7).
 * https://oeis.org/A178704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = sum_{k=0}^n floor(3^k/7)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The series can be computed without ever forming 3^k, using a
 * recurrence for the partial quotients:
 *
 *   f(k)     = floor(3^k/7),
 *   r(k)     = 3^k mod 7
 *
 *   r(k+1)   = (3 * r(k)) mod 7
 *   f(k+1)   = 3 * f(k) + floor(3 * r(k)/7)
 *
 * The sum a(n) is accumulated on the fly.  This keeps the
 * intermediate numbers small and works correctly for all
 * sequence values that fit into seq_t. */

seq_t a(idx_t n)
{
    seq_t sum = 0;          /* running total of a(n) */
    seq_t f   = 0;          /* current f(k) = floor(3^k/7) */
    unsigned r = 1;         /* current r(k) = 3^k mod 7, 0 <= r < 7 */

    for (idx_t k = 0; k <= n; ++k) {
        sum += f;            /* add current floor value */

        /* After the last required term, stop without
         * computing the next f/r; this prevents overflow
         * of f when f*3 would exceed the capacity of seq_t. */
        if (k == n) break;

        unsigned r_next = (3 * r) % 7;         /* r(k+1) */
        unsigned c = (3 * r) / 7;              /* small increment for f(k+1) */
        f = f * 3 + c;                         /* f(k+1) */
        r = r_next;                            /* advance r */
    }

    return sum;
}
