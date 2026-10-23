/*
 * A198095  a(n) is the smallest number k such that d(1)*1! + d(2)*2! + ... +
 *          d(p)*p! = n^2, where d(i) are the decimal digits of k.
 * https://oeis.org/A198095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest k such that Σ_{i=1}^{p} d(i)*i! = n² * where
 *            d(i) are the decimal digits of k and p = number of digits of k.
 *            * * The algorithm searches for k digit by digit, trying the
 *            smallest * possible leading digits first. For each target n² it
 *            iterates over * possible lengths p (1…11). At ea...
 * Range:     u8 n <= 8, u16 n <= 37, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* maximum number of digits needed for n ≤ 10 000
 * 12! > 10⁸, so lengths > 11 cannot satisfy n² ≤ 10⁸.      */
#define MAX_P 11

static seq_t fact[MAX_P + 1];
static bool fact_computed = false;

/* forward declaration of recursive search */
static bool dfs(int pos, int p, seq_t sum, seq_t k,
                seq_t target, seq_t *res);

/*--- public sequence function ------------------------------------------*/
seq_t a(idx_t n)
{
    if (!fact_computed) {
        fact[0] = (seq_t)1;           /* unused */
        fact[1] = (seq_t)1;
        for (int i = 2; i <= MAX_P; ++i)
            fact[i] = fact[i - 1] * (seq_t)i;
        fact_computed = true;
    }

    seq_t N = (seq_t)n;
    seq_t target = N * N;                     /* n² in seq_t */
    seq_t result = 0;

    /* try lengths 1 … MAX_P, the smallest length will yield the smallest k */
    for (int p = 1; p <= MAX_P; ++p) {
        if (dfs(1, p, (seq_t)0, (seq_t)0, target, &result))
            return result;
    }
    /* The sequence is defined for all n; this point should never be reached. */
    return (seq_t)0;
}

/*--- recursive depth‑first search --------------------------------------*/
static bool dfs(int pos, int p, seq_t sum, seq_t k,
                seq_t target, seq_t *res)
{
    if (pos > p) {                          /* all digits chosen */
        if (sum == target) {
            *res = k;
            return true;
        }
        return false;
    }

    /* determine minimal digit to try to avoid leading zeros (except length 1) */
    int start = (pos == 1) ? 1 : 0;

    /* pre‑compute maximum remaining contribution from positions > pos */
    seq_t max_rem = (seq_t)0;
    for (int j = pos + 1; j <= p; ++j)
        max_rem += fact[j] * (seq_t)9;

    for (int d = start; d <= 9; ++d) {
        seq_t add = fact[pos] * (seq_t)d;
        seq_t new_sum = sum + add;
        if (new_sum > target)               /* further digits will only be larger */
            break;

        /* prune if even with all future digits maximal we cannot reach target */
        if (new_sum + max_rem < target)
            continue;

        seq_t new_k = k * (seq_t)10 + (seq_t)d;
        if (dfs(pos + 1, p, new_sum, new_k, target, res))
            return true;
    }
    return false;
}
