/*
 * A033314  Least D in the Pellian x^2 - D*y^2 = 1 for which x has least
 *          solution n.
 * https://oeis.org/A033314
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the least D such that the Pell equation x² – D·y² = 1 *
 *            has fundamental solution x = n. Sequence is defined for n ≥ 2
 * Range:     u8 n <= 8, u16 n <= 256, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#include "../utils/floor_sqrt.h"

/* Return true iff the Pell equation x² – D·y² = 1 has fundamental
 * solution x = n_seq.  The test stops as soon as the convergent
 * exceeds n_seq or the fundamental solution is found. */
static bool pell_has_solution_n(seq_t D, seq_t n_seq)
{
    if (D < 2) return false;          /* no Pell equation for D ≤ 1 */

    seq_t a0 = floor_sqrt(D);
    if (a0 * a0 == D) return false;   /* D is a perfect square */

    seq_t m   = 0;
    seq_t d   = 1;
    seq_t a   = a0;
    seq_t p_prev = 1, p_curr = a0;
    seq_t q_prev = 0, q_curr = 1;

    while (1) {
        /* next term of the continued‑fraction expansion of sqrt(D) */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        seq_t p_next = a * p_curr + p_prev;
        seq_t q_next = a * q_curr + q_prev;

        seq_t lhs = p_next * p_next - D * q_next * q_next;

        if (p_next == n_seq && lhs == 1)
            return true;                /* found the required fundamental solution */

        if (lhs == 1)
            return false;               /* reached fundamental solution > n_seq */

        if (p_next > n_seq)
            return false;               /* cannot be n_seq anymore */

        p_prev = p_curr; p_curr = p_next;
        q_prev = q_curr; q_curr = q_next;
    }
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;                /* defined only for n ≥ 2 */

    seq_t n_seq = (seq_t)n;
    seq_t N = n_seq * n_seq - 1;         /* n² – 1, fits comfortably in seq_t */

    seq_t sqrtN = floor_sqrt(N);
    idx_t kmax = (idx_t)sqrtN;           /* k ≤ √N, and for n ≤ 10000 this fits in idx_t */

    for (idx_t k = kmax; k > 0; --k) {
        seq_t sqk = (seq_t)k * k;
        if (N % sqk != 0) continue;     /* k² must divide N */

        seq_t D = N / sqk;               /* candidate D = (n²–1)/k² */

        seq_t sqrtD = floor_sqrt(D);
        if (sqrtD * sqrtD == D) continue;   /* D cannot be a perfect square */

        if (pell_has_solution_n(D, n_seq))
            return D;                    /* minimal D found */
    }
    return 0;                            /* should never happen for n ≥ 2 */
}
