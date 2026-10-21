/*
 * A072648  a(n) = [log_{Phi}(n*sqrt(5))], where log_{Phi} is logarithm in the
 *          base Phi ( = (sqrt(5)+1)/2) and [] stands for the floor function.
 * https://oeis.org/A072648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(log_{Φ}(n * sqrt(5)))
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for OEIS sequence A072648.
 * The function uses only integer arithmetic.  It evaluates the inequality
 *   Φ^k  ≤  n·√5  <  Φ^(k+1)
 * which is equivalent to
 *   L_k^2  ≤  5 · (2n – F_k)^2        (L_k Lucas, F_k Fibonacci).
 * The largest k satisfying the inequality is returned.  The loop
 * progresses with the Fibonacci/Lucas recurrences
 *    F_{k+1} = F_k + F_{k-1}
 *    L_{k+1} = L_k + L_{k-1}
 * and uses 128‑bit integer arithmetic for safety.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;

    seq_t ans = 0;

    /* k = 0 : F0 = 0, L0 = 2 */
    {
        unsigned __int128 Fk = 0, Lk = 2;
        unsigned __int128 two_n = (unsigned __int128)n << 1;
        if (Fk <= two_n) {
            unsigned __int128 diff = two_n - Fk;
            unsigned __int128 rhs = (unsigned __int128)5 * diff * diff;
            unsigned __int128 lhs = Lk * Lk;
            if (lhs <= rhs) ans = 0;
        }
    }

    /* k = 1 : F1 = 1, L1 = 1 */
    {
        unsigned __int128 Fk = 1, Lk = 1;
        unsigned __int128 two_n = (unsigned __int128)n << 1;
        if (Fk <= two_n) {
            unsigned __int128 diff = two_n - Fk;
            unsigned __int128 rhs = (unsigned __int128)5 * diff * diff;
            unsigned __int128 lhs = Lk * Lk;
            if (lhs <= rhs) ans = 1;
        }
    }

    /* prepare Fibonacci and Lucas for k ≥ 2 */
    unsigned __int128 F_prev = 0;  /* F0 */
    unsigned __int128 F_curr = 1;  /* F1 */
    unsigned __int128 L_prev = 2;  /* L0 */
    unsigned __int128 L_curr = 1;  /* L1 */
    seq_t k = 2;

    while (1) {
        /* compute next Fibonacci and Lucas numbers */
        unsigned __int128 F_next = F_prev + F_curr;  /* Fk */
        unsigned __int128 L_next = L_prev + L_curr;  /* Lk */

        unsigned __int128 Fk = F_next;
        unsigned __int128 Lk = L_next;

        unsigned __int128 two_n = (unsigned __int128)n << 1;
        if (Fk > two_n) break;          /* 2n – Fk would be negative */

        unsigned __int128 diff = two_n - Fk;               /* 2n – Fk */
        unsigned __int128 rhs = (unsigned __int128)5 * diff * diff;
        unsigned __int128 lhs = Lk * Lk;

        if (lhs <= rhs) {
            ans = k;                    /* this k satisfies the inequality */
        } else {
            break;                      /* next k would only be larger */
        }

        /* shift for the next iteration */
        F_prev = F_curr;
        F_curr = F_next;
        L_prev = L_curr;
        L_curr = L_next;
        k++;
    }

    return ans;
}
