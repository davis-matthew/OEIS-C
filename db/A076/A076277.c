/*
 * A076277  Number of product signs needed to write all the factorizations of
 *          n with all factors > 1.
 * https://oeis.org/A076277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{factorizations of n} (k-1) where k is the * number
 *            of factors in the factorization. This is equivalent to * a(n) =
 *            Σ_k(k) - A001055(n), where Σ_k(k) is the sum of the numbers * of
 *            factors over all factorizations. The function below computes *
 *            A001055(n) and Σ_k(k) by recursion over un...
 * Range:     u8 n <= 719, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Recursive helper that returns the number of factorizations (F)
 * and the sum of the number of factors over all factorizations (S). */
static void dfs(idx_t n, idx_t min_factor, seq_t *F, seq_t *S) {
    /* One factorization consisting solely of n itself. */
    *F = (seq_t)1;
    *S = (seq_t)1;          /* k = 1 for the factorization [n] */

    /* Explore first factors d >= min_factor up to sqrt(n). */
    for (idx_t d = min_factor; d * d <= n; ++d) {
        if (n % d != 0) continue;          /* not a divisor */

        idx_t m = n / d;                    /* remaining part */
        seq_t Fsub, Ssub;
        dfs(m, d, &Fsub, &Ssub);            /* factorizations of m with min d */

        /* Prepend d to each sub‑factorization: increase K by 1. */
        *F += Fsub;                         /* new factorizations count */
        *S += Fsub + Ssub;                  /* Ssub plus one per factorization */
    }
}

/* ----------------------------------------------------------------------
 * Public function: a(idx_t n)
 * ---------------------------------------------------------------------- */
seq_t a(idx_t n) {
    /* OEIS offset is 1.  a(1)=0 by definition. */
    if (n <= 1) return (seq_t)0;

    seq_t F, S;
    dfs(n, (idx_t)2, &F, &S);                  /* min factor must be >1 */
    return S - F;                              /* sum(k) - count = Σ(k-1) */
}
