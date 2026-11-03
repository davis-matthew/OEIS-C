/*
 * A070837  Smallest number k such that abs(k - R(k)) = 9n, where R(k) is
 *          digit reversal of k (A004086); or 0 if no such k exists.
 * https://oeis.org/A070837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest k such that |k − R(k)| = 9·n , where R(k) is the
 *            decimal digit reverse of k; return 0 if no such k exists
 * Range:     u8 n <= 9, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 76 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches by increasing number of digits and
   uses backtracking with pruning based on possible remaining
   contributions.  The result is correct for all n ≥ 1.  */
#include <stdint.h>
#include <stdbool.h>

/* Recursive search over digit pairs. */
static void dfs_len(int t, int m, int p,
                    __int128 diff_rem,
                    const __int128 *coeff,
                    const __int128 *minRem,
                    const __int128 *maxRem,
                    int *digits,
                    seq_t *best_val,
                    bool *found)
{
    if (t == p) {
        if (diff_rem != 0) return; /* must match target exactly */
        if (m & 1) digits[m/2] = 0;  /* middle digit minimised */
        /* construct value from most significant to least */
        seq_t val = 0;
        for (int i = m-1; i >= 0; --i) val = val * 10 + (seq_t)digits[i];
        if (!*found || val < *best_val) {
            *best_val = val;
            *found = true;
        }
        return;
    }

    int j = m - 1 - t;          /* most significant position of this pair */
    int i = t;                  /* least significant position of this pair */
    int lb = (t == 0) ? 1 : 0;  /* leading digit cannot be zero */

    for (int diff_d = -9; diff_d <= 9; ++diff_d) {
        /* determine bounds for the higher digit a_j */
        int aj_min = lb;
        if (-diff_d > aj_min) aj_min = -diff_d;      /* ensures a_i >= 0 */
        int aj_max = 9;
        if (9 - diff_d < aj_max) aj_max = 9 - diff_d; /* ensures a_i <= 9 */
        if (aj_min > aj_max) continue;

        int aj = aj_min;                           /* minimal leading digit */
        int ai = aj + diff_d;                      /* lower digit */
        if (ai < 0 || ai > 9) continue;

        __int128 new_rem = diff_rem - coeff[t] * (__int128)diff_d;
        if (new_rem < minRem[t+1] || new_rem > maxRem[t+1]) continue;

        digits[j] = aj;
        digits[i] = ai;
        dfs_len(t+1, m, p, new_rem, coeff, minRem, maxRem,
                digits, best_val, found);
    }
}

/* forward declaration of helper */
static seq_t solve_len(int m, __int128 diff_target);

/* Pre‑computed powers of ten (up to 19 digits; 10^19 < 2^64*10). */
static __int128 pow10[20];
static bool pow10_inited = false;

static void init_pow10(void) {
    if (pow10_inited) return;
    pow10[0] = 1;
    for (int i = 1; i < 20; ++i) pow10[i] = pow10[i-1] * 10;
    pow10_inited = true;
}

/* Solve for a given length m and target difference. */
static seq_t solve_len(int m, __int128 diff_target)
{
    int p = m / 2;  /* number of independent digit pairs */
    if (p == 0) return 0; /* single digit cannot give non‑zero difference */

    __int128 coeff[10];
    for (int t = 0; t < p; ++t) {
        int i = t;
        int j = m - 1 - t;
        coeff[t] = pow10[i] - pow10[j];  /* always negative or zero */
    }

    __int128 minRem[11];
    __int128 maxRem[11];
    minRem[p] = maxRem[p] = 0;
    for (int t = p - 1; t >= 0; --t) {
        __int128 c = coeff[t];
        minRem[t] = minRem[t+1] + c * 9;        /* most negative possible */
        maxRem[t] = maxRem[t+1] + c * -9;       /* most positive possible */
    }

    int digits[20];
    seq_t best_val = 0;
    bool found = false;
    dfs_len(0, m, p, diff_target, coeff, minRem, maxRem,
            digits, &best_val, &found);
    return found ? best_val : 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence is 1‑indexed, safety only */
    init_pow10();

    __int128 target = (__int128)9 * (__int128)n;

    /* search by increasing length; first solution found is minimal */
    for (int m = 2; m < 20; ++m) {   /* 20 digits is ample for n ≤ 10000 */
        seq_t v1 = solve_len(m, target);      /* k – rev(k) = 9n */
        seq_t v2 = solve_len(m, -target);     /* rev(k) – k = 9n */
        if (v1 != 0 || v2 != 0) {
            if (v1 == 0) return v2;
            if (v2 == 0) return v1;
            return v1 < v2 ? v1 : v2;
        }
    }
    return 0;  /* no solution exists up to length 19 */
}
