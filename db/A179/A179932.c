/*
 * A179932  Number of distinct positive integers that can be formed with the
 *          decimal digits of n.
 * https://oeis.org/A179932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A179932:
 * Number of distinct positive integers that can be formed with the decimal
 * digits of n.
 *
 * The algorithm enumerates all non‑empty multisets of the digits of n,
 * counts how many distinct permutations give a positive integer
 * (i.e. the leading digit is not zero), and sums over all multisets.
 * This directly follows the definition of the sequence.
 */

#include <stddef.h>   /* for size_t */

/* Forward declaration of a recursive helper. */
static void dfs(const unsigned c[10], int idx, unsigned r[10], unsigned sum,
                seq_t *total, const seq_t fact[]);

/* Recursive depth‑first search over digit multiplicities. */
static void dfs(const unsigned c[10], int idx, unsigned r[10], unsigned sum,
                seq_t *total, const seq_t fact[])
{
    if (idx == 10) {
        if (sum == 0) return;          /* empty multiset, no numbers */
        if (sum == r[0]) return;       /* all digits zero → number 0 */

        seq_t subtotal = 0;
        for (int v = 1; v < 10; ++v) { /* leading digit must be non‑zero */
            if (r[v] == 0) continue;

            /* Count permutations with v as the first digit */
            seq_t denom = 1;
            for (int d = 0; d < 10; ++d) {
                unsigned mult = r[d];
                if (d == v) mult--;     /* one instance used as leading digit */
                denom *= fact[mult];
            }
            seq_t numer = fact[sum - 1];
            subtotal += numer / denom;
        }
        *total += subtotal;
        return;
    }

    for (unsigned k = 0; k <= c[idx]; ++k) {
        r[idx] = k;
        dfs(c, idx + 1, r, sum + k, total, fact);
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Count decimal digits of n. */
    unsigned c[10] = {0};
    uint64_t tmp = n;
    if (tmp == 0) {
        c[0] = 1;
    } else {
        while (tmp != 0) {
            c[tmp % 10] += 1;
            tmp /= 10;
        }
    }

    /* Total number of digits. */
    unsigned d = 0;
    for (int i = 0; i < 10; ++i) d += c[i];

    /* Factorial table up to d (d ≤ 20 for uint64_t). */
    seq_t fact[21];
    fact[0] = (seq_t)1;
    for (unsigned i = 1; i <= d; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }

    /* Enumerate all digit multisets. */
    seq_t result = (seq_t)0;
    unsigned r[10] = {0};
    dfs(c, 0, r, 0, &result, fact);

    return result;
}
