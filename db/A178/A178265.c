/*
 * A178265  Number of collinear point 7-tuples in an n X n X n cubical grid.
 * https://oeis.org/A178265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σ_{m=7}^{n} C(m‑2,5)
 *                * Σ_{d|((n‑1)/(m‑1))} μ(d)
 *                  * (4*A^3 + 6*n*A^2 + 3*n²*A)
 *   where
 *     U   = ⌊(n‑1)/(m‑1)⌋,
 *     P   = ⌊U/d⌋,
 *     A   = P*n – (m‑1)d·P(P+1)/2.
 *
 * The inner summation uses the Möbius function μ to
 * eliminate non‑primitive directions.  The outer sums
 * are modest; the whole algorithm runs in O(n log n).
 */

seq_t a(idx_t n)
{
    /* Handle trivial cases */
    if (n < 7)
        return 0;

    /* Largest factor we ever need to sieve for μ(d) */
    int Umax = (int)((n - 1) / 6);          /* because m ≥ 7  →  m‑1 ≥ 6 */
    static int mu[2000];                    /* Umax ≤ 1666 for n ≤ 10000 */
    /* initialise μ to 1 */
    int i;
    for (i = 0; i <= Umax; ++i)
        mu[i] = 1;

    /* Classic sieve for Möbius */
    for (i = 2; i <= Umax; ++i) {
        int p = i;
        /* check if p is prime: sieve of Eratosthenes style */
        int is_prime = 1;
        int q;
        for (q = 2; q * q <= p; ++q) {
            if (p % q == 0) {
                is_prime = 0;
                break;
            }
        }
        if (!is_prime)
            continue;
        /* flip sign for each multiple of p */
        for (q = p; q <= Umax; q += p)
            mu[q] = -mu[q];
        /* zero multiples of p² */
        int sq = p * p;
        if (sq <= Umax)
            for (q = sq; q <= Umax; q += sq)
                mu[q] = 0;
    }

    seq_t total = 0;

    /* Main summation over line lengths m = 7 … n */
    for (idx_t m = 7; m <= n; ++m) {
        /* U = ⌊(n‑1)/(m‑1)⌋ – maximum possible d */
        idx_t U = (n - 1) / (m - 1);
        if (U == 0)      /* cannot happen for m ≤ n, but keep defensive */
            break;

        /* binomial coefficient C(m‑2,5) computed multiplicatively */
        seq_t bin = ((seq_t)(m - 2)) * (m - 3) * (m - 4) * (m - 5) * (m - 6)
                    / (seq_t)120;   /* 5! = 120 */

        /* Möbius summation */
        seq_t pos = 0, neg = 0;           /* contributions with μ = +1 / –1 */

        int Uint = (int)U;
        for (int d = 1; d <= Uint; ++d) {
            int mud = mu[d];
            if (mud == 0)
                continue;

            /* P = ⌊U/d⌋ */
            int P = Uint / d;
            if (P == 0)
                continue;

            seq_t Pseq = (seq_t)P;
            seq_t A = Pseq * (seq_t)n
                      - ((seq_t)(m - 1)) * (seq_t)d
                        * Pseq * (Pseq + 1) / (seq_t)2;

            /* 4*A³ + 6*n*A² + 3*n²*A  (all operations in seq_t) */
            seq_t contrib = 4 * A * A * A
                            + 6 * (seq_t)n * A * A
                            + 3 * (seq_t)n * (seq_t)n * A;

            if (mud > 0)
                pos += contrib;
            else
                neg += contrib;
        }

        seq_t S = pos - neg;     /* inner Möbius sum */
        total += bin * S;
    }

    return total;
}
