/*
 * A003135  n! is a nontrivial product of factorials. It is conjectured that
 *          the list is complete.
 * https://oeis.org/A003135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, bref, more, hard
 * Formula:   search. For each m, try every x with P(m) <= x <= m-2, where
 *            P(m) is the largest prime <= m. Test by depth-first search
 *            whether m!/x! = (x+1)*...*m factors as a product of factorials
 *            k! with k <= x, picking k in nonincreasing order. Prime-exponent
 *            vectors prune the search
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   a3135_vpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>
#include "../utils/a3135_vpf.h"

#define A3135_LIM 64

static int a3135_pr[A3135_LIM];
static int a3135_np = 0;
static int a3135_rem[A3135_LIM];

static void a3135_init(void) {
    if (a3135_np) return;
    for (int i = 2; i < A3135_LIM; i++) {
        bool ok = true;
        for (int d = 2; d * d <= i; d++) if (i % d == 0) { ok = false; break; }
        if (ok) a3135_pr[a3135_np++] = i;
    }
}

static bool a3135_dfs(int maxk) {
    int q = 0;
    for (int i = a3135_np - 1; i >= 0; i--)
        if (a3135_rem[i] > 0) { q = a3135_pr[i]; break; }
    if (q == 0) return true;
    for (int k = maxk; k >= q; k--) {
        bool ok = true;
        for (int i = 0; i < a3135_np && a3135_pr[i] <= k; i++)
            if (a3135_vpf(k, a3135_pr[i]) > a3135_rem[i]) { ok = false; break; }
        if (!ok) continue;
        for (int i = 0; i < a3135_np && a3135_pr[i] <= k; i++)
            a3135_rem[i] -= a3135_vpf(k, a3135_pr[i]);
        bool r = a3135_dfs(k);
        for (int i = 0; i < a3135_np && a3135_pr[i] <= k; i++)
            a3135_rem[i] += a3135_vpf(k, a3135_pr[i]);
        if (r) return true;
    }
    return false;
}

static bool a3135_is_term(int m) {
    int P = 2;
    for (int i = 0; i < a3135_np; i++) if (a3135_pr[i] <= m) P = a3135_pr[i];
    for (int x = P; x <= m - 2; x++) {
        for (int i = 0; i < a3135_np; i++)
            a3135_rem[i] = a3135_vpf(m, a3135_pr[i]) - a3135_vpf(x, a3135_pr[i]);
        if (a3135_dfs(x)) return true;
    }
    return false;
}

static int a3135_found[16];
static int a3135_cnt = 0;
static int a3135_next = 3;

seq_t a(idx_t n) {
    a3135_init();
    while ((idx_t)a3135_cnt < n && a3135_next < A3135_LIM) {
        if (a3135_is_term(a3135_next) && a3135_cnt < 16)
            a3135_found[a3135_cnt++] = a3135_next;
        a3135_next++;
    }
    if (n < 1 || n > (idx_t)a3135_cnt) return 0;
    return (seq_t)a3135_found[n - 1];
}
