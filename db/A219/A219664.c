/*
 * A219664  Repeating part of A220664: First differences of the numbers given
 *          as concatenation of permutations of (1,...,m) for sufficiently
 *          large m.
 * https://oeis.org/A219664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 719, u32 n <= 5039, u64 n <= 5039,
 *            u128 n <= 5039, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) equals the first difference between consecutive lexicographically ordered permutations
   of digits 1..m, where m is the smallest integer with m! > n,
   interpreted as decimal numbers. */
#include <stdbool.h>

static bool next_permutation(idx_t *perm, idx_t m)
{
    /* Find the largest i such that perm[i] < perm[i+1] */
    int i;
    for (i = (int)m - 2; i >= 0; --i) {
        if (perm[i] < perm[i + 1])
            break;
    }
    if (i < 0) /* last permutation */
        return false;

    /* Find the largest j > i such that perm[i] < perm[j] */
    int j;
    for (j = (int)m - 1; j > i; --j) {
        if (perm[i] < perm[j])
            break;
    }

    /* Swap perm[i] and perm[j] */
    idx_t tmp = perm[i];
    perm[i] = perm[j];
    perm[j] = tmp;

    /* Reverse the suffix perm[i+1 .. m-1] */
    int l = i + 1, r = (int)m - 1;
    while (l < r) {
        tmp = perm[l];
        perm[l] = perm[r];
        perm[r] = tmp;
        ++l;
        --r;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) /* undefined for the offset but return 0 safely */
        return (seq_t)0;

    /* Find minimal m such that m! > n */
    idx_t fact = 1;
    idx_t m = 1;
    while (fact <= n) {
        ++m;
        fact *= m;          /* m! grows quickly; overflow not a problem for n ≤ 10 000 */
    }

    /* 20 is more than enough for n up to 10 000 (9! > 10 000) */
    idx_t perm[20];
    for (idx_t i = 0; i < m; ++i)
        perm[i] = i + 1;          /* first lexicographic permutation */

    /* Compute value of the first permutation */
    seq_t prev = 0;
    for (idx_t i = 0; i < m; ++i)
        prev = prev * 10 + (seq_t)perm[i];

    seq_t answer = 0;
    for (idx_t step = 1; step <= n; ++step) {
        if (!next_permutation(perm, m))
            break;               /* should never happen for valid n */

        seq_t val = 0;
        for (idx_t i = 0; i < m; ++i)
            val = val * 10 + (seq_t)perm[i];

        seq_t diff = val - prev;
        if (step == n)
            answer = diff;
        prev = val;
    }
    return answer;
}
