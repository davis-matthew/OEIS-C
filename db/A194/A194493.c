/*
 * A194493  Number of ways to arrange 3 nonattacking queens on the lower
 *          triangle of an n X n board.
 * https://oeis.org/A194493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^6/48 - 11*n^5/48 + 15*n^4/16 - 241*n^3/144 + 17*n^2/16
 *            - 17*n/144 + (n^2/8 - 9*n/8 + 17/8)*floor(n/2) + 2/3*floor(n/3)
 *            The implementation rewrites this as an integer numerator divided
 *            by 144. All intermediate results remain non‑negative, so
 *            unsigned arithmetic is safe
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 35, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>


seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    seq_t n5 = n4 * nn;
    seq_t n6 = n5 * nn;

    /* floor divisions */
    seq_t B = (seq_t)(n / 2);
    seq_t C = (seq_t)(n / 3);

    /* The sign of (n^2 - 9*n + 17) is handled explicitly. */
    int64_t t = (int64_t)n2 - 9 * (int64_t)n + 17;

    /* Positive contributions */
    seq_t pos = 3 * n6 + 135 * n4 + 153 * n2 + 96 * C;
    if (t >= 0)
        pos += 18 * (seq_t)t * B;

    /* Negative contributions */
    seq_t neg = 33 * n5 + 241 * n3 + 17 * nn;
    if (t < 0)
        neg += 18 * (seq_t)(-t) * B;

    /* Numerator */
    seq_t num = pos - neg;
    /* Denominator 144 */
    return num / 144;
}
