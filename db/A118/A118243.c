/*
 * A118243  Triangle generated from Pell polynomials.
 * https://oeis.org/A118243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) = V_{c}(r - c + 2) with t_r = r(r+1)/2, c = n - t_r *
 *            V_0(s) = 1, * V_1(s) = s, * V_m(s) = s*V_{m-1}(s) + V_{m-2}(s)
 *            (m > 1) * The sequence is read row‑major from the triangle with
 *            rows * r = 0,1,2,... and columns c = 0..r. * Example: rows
 *            produced are * 1; * 1, 2; * 1, 3, 5; * 1, 4, 10, 1...
 * Range:     u8 n <= 24, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include guards for any required standard headers */
#include <stdint.h>

/* Exported sequence function */
seq_t a(idx_t n)
{
    /* Find row r such that t_r <= n < t_{r+1}  where t_r = r(r+1)/2 */
    idx_t r = 0;
    while ( ((r + 1ULL) * (r + 2ULL) / 2ULL) <= n )
        r++;

    /* Column index within the row */
    idx_t t_r = r * (r + 1ULL) / 2ULL;
    idx_t c   = n - t_r;

    /* Compute V_c(s) where s = r - c + 2 */
    idx_t s = r - c + 2ULL;

    /* Base cases */
    if (c == 0)
        return (seq_t)1;          /* V_0(s) = 1 */

    /* V_1(s) = s */
    seq_t prev = (seq_t)1;         /* V_0(s) */
    seq_t curr = (seq_t)s;         /* V_1(s) */
    if (c == 1)
        return curr;

    /* Recurrence: V_m = s*V_{m-1} + V_{m-2} */
    seq_t next;
    for (idx_t i = 2; i <= c; ++i) {
        next = (seq_t)s * curr + prev;
        prev = curr;
        curr = next;
    }

    return curr;   /* V_c(s) */
}
