/*
 * A041718  Numerators of continued fraction convergents to sqrt(379).
 * https://oeis.org/A041718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the nth convergent to sqrt(379) For n >= 0,
 *            with a(0)=19. The numerators are computed by the standard
 *            continued‑fraction recurrence: p_k = a_k * p_{k-1} + p_{k-2}
 *            where a_k are the partial quotients of sqrt(379)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 15, u64 n <= 32, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Utility: compute floor(sqrt(n)) for an unsigned 64‑bit integer.
   Uses binary search with 128‑bit intermediate multiplication
   to avoid overflow. */
static inline idx_t
floor_sqrt_u64(idx_t n)
{
    idx_t low  = 0;
    idx_t high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    /* Constant for the continued fraction of sqrt(379). */
    const idx_t N  = (idx_t)379;
    const idx_t a0 = floor_sqrt_u64(N);          /* integer part */

    /* State for the continued‑fraction algorithm */
    idx_t m = 0;               /* m_k   */
    idx_t d = 1;               /* d_k   */
    idx_t a = a0;              /* a_k   */

    /* Numerators: p_{-1}=1, p_{-2}=0   */
    seq_t prev = (seq_t)1;     /* p_{k-1}  */
    seq_t curr = (seq_t)0;     /* p_{k-2}  */

    for (idx_t k = 0; k <= n; ++k) {
        /* Compute numerator p_k */
        seq_t pk = (seq_t)a * prev + curr;

        /* On the last iteration return the required term */
        if (k == n) return pk;

        /* Advance to the next partial quotient a_{k+1} */
        m = d * a - m;
        __int128 mm = (__int128)m * m;
        d = (N - mm) / d;
        a = (a0 + m) / d;

        /* Shift for next iteration */
        curr = prev;
        prev = pk;
    }

    /* Should not reach here */
    return (seq_t)0;
}
