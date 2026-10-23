/*
 * A031424  Least term in period of continued fraction for sqrt(n), as n runs
 *          through the nonsquares (A000037).
 * https://oeis.org/A031424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Least term in the period of the continued fraction of sqrt(D),
   where D is the n‑th non‑square number. */
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    /* --- Find the n-th non‑square D ---------------------------------- */
    idx_t low  = n;
    idx_t high = n + _floor_isqrt_u64(n) + 5;
    if (high <= low) high = low + 10;

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t s   = _floor_isqrt_u64(mid);
        idx_t nonsq = mid - s;         /* # of non‑squares ≤ mid        */
        if (nonsq < n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t D = low;                          /* the desired non‑square      */

    /* --- Continued‑fraction expansion of sqrt(D) --------------------- */
    idx_t a0 = _floor_isqrt_u64(D);          /* integer part of sqrt(D)    */

    /*  Initial triple (m_k, d_k, a_k) for k = 0.                      */
    idx_t prev_m = 0, prev_d = 1, prev_a = a0;

    seq_t least = 0;                         /* will be set on first term  */

    while (1) {
        idx_t m_next = prev_d * prev_a - prev_m;
        idx_t d_next = (D - m_next * m_next) / prev_d;
        idx_t a_next = (a0 + m_next) / d_next;

        seq_t a_next_seq = (seq_t)a_next;
        if (least == (seq_t)0)                 /* first generated term     */
            least = a_next_seq;
        else if (a_next_seq < least)
            least = a_next_seq;

        /*  Period ends when a term equals 2*a0.                         */
        if (a_next == 2 * a0)
            break;

        prev_m = m_next;
        prev_d = d_next;
        prev_a = a_next;
    }

    return least;
}
