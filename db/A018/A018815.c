/*
 * A018815  Number of lines through exactly 8 points of an n X n grid of
 *          points.
 * https://oeis.org/A018815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of lines through exactly 8 points of an n X n grid
 *            of points. Computation is by enumerating primitive direction
 *            vectors (dx,dy) with 0 < dx,dy ≤ (n-1)/7, counting minimal lines
 *            of that direction which contain exactly 8 grid points. Then the
 *            count for negative slopes is obtained by s...
 * Range:     u8 n <= 23, u16 n <= 109, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t ugcd(idx_t a, idx_t b)
{
    while (b)
    {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 8) return 0;

    seq_t result = 0;
    unsigned long long N = n;               /* use 64-bit for intermediate math */

    /* vertical and horizontal lines contain n points.  They contribute only
       when n == 8, each providing 8 such lines. */
    if (n == 8)
    {
        result += 16;  /* 8 vertical + 8 horizontal */
    }

    /* count all lines with positive slopes; negative slopes will be
       accounted for by multiplying the result by 2 at the end. */
    seq_t pos_lines = 0;
    idx_t M = (n == 0) ? 0 : (n - 1) / 7;

    for (idx_t dx = 1; dx <= M; ++dx)
    {
        for (idx_t dy = 1; dy <= M; ++dy)
        {
            if (ugcd(dx, dy) != 1) continue;

            /* counts of x with p = floor((n-1 - x)/dx) == 7 and p >= 7 */
            unsigned long long p_ge7 = (N > 7ULL * dx) ? (N - 7ULL * dx) : 0ULL;
            unsigned long long left_p = (N > 8ULL * dx) ? (N - 8ULL * dx) : 0ULL;
            unsigned long long right_p = (N > 7ULL * dx) ? (N - 1ULL - 7ULL * dx) : (unsigned long long)-1;
            unsigned long long p_eq = (left_p <= right_p) ? (right_p - left_p + 1ULL) : 0ULL;

            /* same for y */
            unsigned long long q_ge7 = (N > 7ULL * dy) ? (N - 7ULL * dy) : 0ULL;
            unsigned long long left_q = (N > 8ULL * dy) ? (N - 8ULL * dy) : 0ULL;
            unsigned long long right_q = (N > 7ULL * dy) ? (N - 1ULL - 7ULL * dy) : (unsigned long long)-1;
            unsigned long long q_eq = (left_q <= right_q) ? (right_q - left_q + 1ULL) : 0ULL;

            /* counts restricted to x >= dx and y >= dy (non‑minimal lines) */
            unsigned long long p_ge7_in = (N > 8ULL * dx) ? (N - 8ULL * dx) : 0ULL;
            unsigned long long left_p_in = (N > 8ULL * dx) ? (N - 8ULL * dx) : 0ULL;
            unsigned long long left_p_incl = ((unsigned long long)dx > left_p_in) ? (unsigned long long)dx : left_p_in;
            unsigned long long right_p_incl = (N > 7ULL * dx) ? (N - 1ULL - 7ULL * dx) : (unsigned long long)-1;
            unsigned long long p_eq_in = (left_p_incl <= right_p_incl) ? (right_p_incl - left_p_incl + 1ULL) : 0ULL;

            unsigned long long q_ge7_in = (N > 8ULL * dy) ? (N - 8ULL * dy) : 0ULL;
            unsigned long long left_q_in = (N > 8ULL * dy) ? (N - 8ULL * dy) : 0ULL;
            unsigned long long left_q_incl = ((unsigned long long)dy > left_q_in) ? (unsigned long long)dy : left_q_in;
            unsigned long long right_q_incl = (N > 7ULL * dy) ? (N - 1ULL - 7ULL * dy) : (unsigned long long)-1;
            unsigned long long q_eq_in = (left_q_incl <= right_q_incl) ? (right_q_incl - left_q_incl + 1ULL) : 0ULL;

            /* total pairs with min(p,q)=7 */
            unsigned long long total_pairs = p_eq * q_ge7 + p_ge7 * q_eq;
            if (total_pairs >= p_eq * q_eq) total_pairs -= p_eq * q_eq;
            else total_pairs = 0;

            /* pairs where both x>=dx and y>=dy (non‑minimal) */
            unsigned long long bad_pairs = p_eq_in * q_ge7_in + p_ge7_in * q_eq_in;
            if (bad_pairs >= p_eq_in * q_eq_in) bad_pairs -= p_eq_in * q_eq_in;
            else bad_pairs = 0;

            pos_lines += (seq_t)(total_pairs - bad_pairs);
        }
    }

    /* account for negative slopes, symmetric to positive ones */
    result += pos_lines * 2ULL;

    return result;
}
