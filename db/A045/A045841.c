/*
 * A045841  Number of distinct odd numbers formed from the digits of n.
 * https://oeis.org/A045841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A045841: number of distinct odd numbers that can be formed
   from the decimal digits of n. */
#include <stdint.h>
#include <stdbool.h>

/* Recursive helper: enumerates all distinct digit-count subsets */
static void dfs(int pos, int len, int *cnt, const int *freq,
                seq_t *res, const seq_t *fact)
{
    if (pos == 10) {
        if (len == 0) return;              /* skip empty subset */
        seq_t subtotal = 0;

        if (len == 1) {                    /* single‑digit subset */
            for (int d = 1; d <= 9; d += 2)
                if (cnt[d] > 0) { subtotal = 1; break; }
        } else {
            /* two or more digits: choose last digit odd and first digit non‑zero */
            for (int dl = 1; dl <= 9; dl += 2) {
                if (cnt[dl] == 0) continue;
                cnt[dl]--;
                for (int df = 1; df <= 9; ++df) {
                    if (cnt[df] == 0) continue;
                    cnt[df]--;
                    int rem = len - 2;
                    seq_t perms = fact[rem];
                    for (int t = 0; t < 10; ++t)
                        if (cnt[t] > 1) perms /= fact[cnt[t]];
                    subtotal += perms;
                    cnt[df]++;            /* restore first digit count */
                }
                cnt[dl]++;                  /* restore last digit count */
            }
        }
        *res += subtotal;
        return;
    }

    /* iterate over possible counts of digit 'pos' in the subset */
    for (int k = 0; k <= freq[pos]; ++k) {
        cnt[pos] = k;
        dfs(pos + 1, len + k, cnt, freq, res, fact);
        cnt[pos] = 0;   /* reset for next choice */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* count frequency of each decimal digit in n */
    int freq[10] = {0};
    uint64_t tmp = (uint64_t)n;
    while (tmp > 0) {
        int d = (int)(tmp % 10);
        freq[d]++;
        tmp /= 10;
    }

    /* precompute factorials up to 19! (maximum number of digits) */
    static seq_t fact[20];
    static bool fact_done = false;
    if (!fact_done) {
        fact[0] = 1;
        for (int i = 1; i <= 19; ++i)
            fact[i] = fact[i - 1] * (seq_t)i;
        fact_done = true;
    }

    int cnt[10] = {0};          /* used counts for the current subset */
    seq_t result = 0;
    dfs(0, 0, cnt, freq, &result, fact);
    return result;
}
