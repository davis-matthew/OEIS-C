/*
 * A186382  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f(i)=4i and g(j)=j(j+1)/2 (triangular
 *          number). Complement of A186381.
 * https://oeis.org/A186382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n + floor(n^2/8) + floor((n-1)/8) + (n mod 8 == 6 ? 1 : 0)
 *  (OEIS A186382 – Adjusted joint rank sequence of f(i)=4i and g(j)=j(j+1)/2,
 *   with f(i) after g(j) when equal; complement of A186381.)
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic. */
    seq_t sn = (seq_t)n;

    /* Square of n.  The multiplication is performed in seq_t
       which may be arbitrary precision or any unsigned size as
       required by the build. */
    seq_t sq = sn * sn;

    /* floor(n^2 / 8) */
    seq_t q1 = sq / 8;

    /* floor((n-1) / 8) */
    seq_t q2 = (sn - 1) / 8;

    /* Base result */
    seq_t result = sn + q1 + q2;

    /* Add one if n mod 8 == 6 */
    if ((n & 7) == 6)
        result += 1;

    return result;
}
