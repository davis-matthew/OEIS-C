/*
 * A240490  Number of partitions of n containing m(5) as a part, where m
 *          denotes multiplicity.
 * https://oeis.org/A240490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1, 5k≤n} q(n-6k) + [n≥25]·q(n-25) where q(m) =
 *            number of partitions of m into parts ≠ 5 (k = m(5) = exact
 *            multiplicity of the part 5)
 * Range:     u8 n <= 22, u16 n <= 49, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*  Pre‑computed partition counts:  
 *     P[m] – unrestricted partitions of m
 *     Q[m] – partitions of m avoiding the part 5
 */
static seq_t *P = NULL, *Q = NULL;
static idx_t pre_max = 0;

/* Build partitions up to n (inclusive) for both P and Q. */
static void ensure_partitions(idx_t n)
{
    if (n <= pre_max) return;

    if (P) free(P);
    if (Q) free(Q);

    P = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    Q = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!P || !Q) { /* memory error – abort */ abort(); }

    /* initialise to zero */
    for (idx_t i = 0; i <= n; ++i) {
        P[i] = 0;
        Q[i] = 0;
    }
    P[0] = 1;
    Q[0] = 1;

    for (idx_t part = 1; part <= n; ++part) {
        /* unrestricted partitions */
        for (idx_t sum = part; sum <= n; ++sum)
            P[sum] += P[sum - part];

        /* partitions avoiding part 5 */
        if (part == 5) continue;
        for (idx_t sum = part; sum <= n; ++sum)
            Q[sum] += Q[sum - part];
    }
    pre_max = n;
}

/* Main routine */
seq_t a(idx_t n)
{
    ensure_partitions(n);

    seq_t result = 0;

    /* handle k==5 separately */
    if (n >= 25) /* n-25 ≥ 0 */
        result += Q[n - 25];

    /* sum over k ≠ 5 */
    idx_t maxk = n / 5;
    for (idx_t k = 1; k <= maxk; ++k) {
        if (k == 5) continue;           /* already counted above */

        /* need at least 6k ≤ n otherwise n-6k would underflow */
        if (n < 6 * k) continue;
        result += Q[n - 6 * k];
    }
    return result;
}
