/*
 * A126687  Maximal value of GCD of 2 distinct n-digit numbers using the same
 *          digits.
 * https://oeis.org/A126687
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 5*10^(n-1) - 71429 for n >= 6, otherwise a(n) is computed
 *            by exhaustive search
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   gcd, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/pow10.h"

/* compute a(n) for n <= 5 by brute force */
static void permute_rec(int cnt[10], int pos, int n_rem, seq_t curr,
                       seq_t *perm, size_t *perm_cnt)
{
    if (n_rem == 0) {
        perm[(*perm_cnt)++] = curr;
        return;
    }
    for (int d = 0; d < 10; d++) {
        if (cnt[d] == 0) continue;
        if (pos == 0 && d == 0) continue;   /* no leading zero */
        cnt[d]--;
        permute_rec(cnt, pos + 1, n_rem - 1, curr * 10 + d,
                    perm, perm_cnt);
        cnt[d]++;                         /* backtrack */
    }
}

static void process_multiset(const int multiset[], int n, seq_t *best)
{
    int cnt[10] = {0};
    for (int i = 0; i < n; i++) cnt[multiset[i]]++;

    /* all zeros -> cannot form a valid n‑digit number */
    if (cnt[0] == n) return;

    seq_t perm[120];          /* n! <= 120 for n <= 5 */
    size_t perm_cnt = 0;
    permute_rec(cnt, 0, n, 0, perm, &perm_cnt);
    if (perm_cnt < 2) return;  /* need two distinct numbers */

    for (size_t i = 0; i < perm_cnt; i++) {
        for (size_t j = i + 1; j < perm_cnt; j++) {
            seq_t g = gcd(perm[i], perm[j]);
            if (g > *best) *best = g;
        }
    }
}

static void gen_multisets_rec(int pos, int last, int n,
                              int *current, seq_t *best)
{
    if (pos == n) {
        process_multiset(current, n, best);
        return;
    }
    for (int d = last; d < 10; d++) {
        current[pos] = d;
        gen_multisets_rec(pos + 1, d, n, current, best);
    }
}

static seq_t compute_small_n(int n)
{
    if (n < 2) return 0;
    seq_t best = 0;
    int current[5];
    gen_multisets_rec(0, 0, n, current, &best);
    return best;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;                /* undefined for n < 2 */

    if (n <= 5) return compute_small_n((int)n);

    /* for n >= 6 use the closed‑form pattern */
    seq_t ten_pow = pow10(n - 1);        /* 10^(n-1) */
    seq_t result = ten_pow * ((seq_t)5);
    result -= (seq_t)71429;
    return result;
}
