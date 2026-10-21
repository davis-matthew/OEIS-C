/*
 * A069018  Smallest square k > 0 such that n*k + 1 is also a square or 0 if
 *          no such term exists, i.e., when n is a square.
 * https://oeis.org/A069018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n is a perfect square, k^2 where k>0 is the smallest
 *            integer such that n*k^2 + 1 is a square (i.e. k is the minimal
 *            positive t solving y^2 - n*t^2 = 1)
 * Range:     u8 n <= 12, u16 n <= 28, u32 n <= 60, u64 n <= 108,
 *            u128 n <= 396, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t floor_sqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq == (unsigned __int128)n) return mid;
        else if (sq < (unsigned __int128)n) low = mid + 1;
        else high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    /* Convert n to seq_t for calculations */
    seq_t n_t = (seq_t)n;

    /* Compute integer sqrt of n */
    idx_t sqrt_i = floor_sqrt_u64(n);
    seq_t sqrt_n = (seq_t)sqrt_i;

    /* If n is a perfect square, result is 0 */
    /* In case seq_t is smaller than n, we rely on arithmetic content */
    if (sqrt_n * sqrt_n == n_t)
        return (seq_t)0;

    /* Continued fraction expansion of sqrt(n) */
    seq_t a0 = sqrt_n;
    seq_t m = (seq_t)0;
    seq_t d = (seq_t)1;
    seq_t a_cur = a0;

    /* Convergent recurrence terms */
    seq_t h_prevprev = (seq_t)0;
    seq_t h_prev = (seq_t)1;
    seq_t k_prevprev = (seq_t)1;
    seq_t k_prev = (seq_t)0;

    /* Iteration variable cache */
    seq_t h_cur, k_cur;

    while (1) {
        /* Compute next convergent */
        h_cur = a_cur * h_prev + h_prevprev;
        k_cur = a_cur * k_prev + k_prevprev;

        /* Check Pell equation h_cur^2 - n * k_cur^2 == 1 */
        seq_t lhs = h_cur * h_cur;
        seq_t rhs = n_t * (k_cur * k_cur);
        seq_t rhs_plus1 = rhs + (seq_t)1;

        if (lhs == rhs_plus1) {
            /* Found minimal t = k_cur; return k = t^2 */
            seq_t result = k_cur * k_cur;
            return result;
        }

        /* Prepare for next iteration */
        h_prevprev = h_prev;  h_prev = h_cur;
        k_prevprev = k_prev;  k_prev = k_cur;

        /* Compute next a in continued fraction */
        seq_t m_next = d * a_cur - m;
        seq_t d_next = (n_t - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a_cur = a_next;
    }
}
