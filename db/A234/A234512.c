/*
 * A234512  Numbers n = d(0)d(1)d(2)...d(r) such that d(i) is the number of
 *          differences |d(i)-d(i-1)| equal to i in n, i = 1,2,...,r.
 * https://oeis.org/A234512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   differential autobiographical numbers (OEIS A234512). No closed
 *            form; found by back‑track search over all possible digit vectors
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#define MAX_L 91   /* maximal possible length: digits 0..9 sum up to 90 */

static idx_t g_target;          /* n to find (1‑based) */
static seq_t g_result;          /* resulting number when found */
static idx_t g_counter;         /* count of solutions found so far */

static bool dfs(idx_t L, int pos, int sum, int *cnt, int *dig)
{
    if (pos >= L) return false;   /* should not happen */

    /* deterministic zeros for positions >= 10 */
    if (pos >= 10) {
        int digit = 0;
        dig[pos] = 0;
        int diff = 0;
        if (pos > 0) {
            diff = abs(digit - dig[pos-1]);
            cnt[diff]++;                /* record the difference */
        }

        /* pruning by sum */
        if (sum > (int)(L-1)) {
            if (pos > 0) cnt[diff]--;
            return false;
        }

        if (pos + 1 < L) {
            if (dfs(L, pos+1, sum, cnt, dig)) {
                if (pos > 0) cnt[diff]--;
                return true;
            }
        } else { /* last position */
            /* check validity of candidate */
            int ok = 1;
            for (int i = 0; i <= 9; ++i) {
                int c = cnt[i];
                if (i < L) {
                    if (c != dig[i]) { ok = 0; break; }
                } else {
                    if (c != 0) { ok = 0; break; }
                }
            }
            if (ok) {
                ++g_counter;
                if (g_counter == g_target) {
                    seq_t val = 0;
                    for (int k = 0; k < L; ++k) val = val*10 + dig[k];
                    g_result = val;
                    if (pos > 0) cnt[diff]--;
                    return true;
                }
            }
        }
        if (pos > 0) cnt[diff]--;
        return false;
    } else {
        int start = (pos == 0 ? 1 : 0);
        for (int digit = start; digit <= 9; ++digit) {
            dig[pos] = digit;
            int diff = 0;
            if (pos > 0) {
                diff = abs(digit - dig[pos-1]);
                cnt[diff]++;                /* record the difference */
            }

            int new_sum = sum + digit;
            int remaining = L - pos - 1;

            /* sum pruning: can we still reach total L-1? */
            if (new_sum <= (int)(L-1) && new_sum + remaining*9 >= (int)(L-1)) {
                /* digit count pruning: a already known digit cannot be exceeded */
                bool ok = true;
                for (int i = 0; i <= pos; ++i) {
                    if (cnt[i] > dig[i]) { ok = false; break; }
                }
                if (ok) {
                    if (pos + 1 < L) {
                        if (dfs(L, pos+1, new_sum, cnt, dig)) {
                            if (pos > 0) cnt[diff]--;
                            return true;
                        }
                    } else { /* last digit */
                        /* check validity */
                        int dd_ok = 1;
                        for (int i = 0; i <= 9; ++i) {
                            int c = cnt[i];
                            if (i < L) {
                                if (c != dig[i]) { dd_ok = 0; break; }
                            } else {
                                if (c != 0) { dd_ok = 0; break; }
                            }
                        }
                        if (dd_ok) {
                            ++g_counter;
                            if (g_counter == g_target) {
                                seq_t val = 0;
                                for (int k = 0; k < L; ++k) val = val*10 + dig[k];
                                g_result = val;
                                if (pos > 0) cnt[diff]--;
                                return true;
                            }
                        }
                    }
                }
            }
            if (pos > 0) cnt[diff]--;        /* backtrack diff counter */
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1, but guard */
    g_target = n;
    g_counter = 0;
    g_result = 0;

    int dig[MAX_L];
    int cnt[10];

    for (idx_t L = 1; L <= MAX_L; ++L) {
        /* clear counts */
        for (int i = 0; i < 10; ++i) cnt[i] = 0;
        for (int i = 0; i < L; ++i) dig[i] = 0;

        if (dfs(L, 0, 0, cnt, dig)) 
            return g_result;   /* found */
    }
    return 0;   /* if not found (should not happen for valid n) */
}
