/*
 * A125181  Triangle read by rows: T(n,k) is the number of Dyck paths of
 *          semilength n whose ascent lengths form the k-th partition of the
 *          integer n; the partitions of n are ordered in the "Mathematica"
 *          ordering.
 * https://oeis.org/A125181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, tabf
 * Formula:   a(n) is the n‑th term of the flattened triangle of T(n,k). For
 *            each row m the partitions of m are listed in Mathematica order.
 *            For a partition p = [a(1)^{e(1)}, …, a(j)^{e(j)}] with k =
 *            e(1)+…+e(j) the corresponding triangle entry is (P) T(m,k) = m!
 *            / [ (m‑k+1)! · e(1)! e(2)! … e(j)! ]. The funct...
 * Range:     u8 n <= 59, u16 n <= 453, u32 n <= 5563, u64 n <= 11731,
 *            u128 n <= 11731, big any n
 * Validated: standard, pass, 95 terms through n = 9222 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required definitions (provided by the build system) */
#include <stdbool.h>

/* v_p(m!), by Legendre */
static idx_t vfact(idx_t m, idx_t p)
{
    idx_t e = 0;
    while (m) { m /= p; e += m; }
    return e;
}

/* T(m,k) = C(m,k-1) (k-1)! / prod mult_i! = m! / ((m-k+1)! prod mult_i!) for a
   partition parts[0..k-1] (non-increasing) with part multiplicities mult_i. It
   is computed from its prime exponents as a plain product, so it is exact
   modulo the width (dividing after the products had wrapped was not). */
static seq_t compute_T(idx_t m, idx_t k, int *parts)
{
    idx_t mult[128], nm = 0, cnt = 1;
    for (idx_t i = 1; i <= k; ++i) {
        if (i < k && parts[i] == parts[i - 1]) { ++cnt; continue; }
        mult[nm++] = cnt;
        cnt = 1;
    }
    seq_t res = 1;
    for (idx_t p = 2; p <= m; ++p) {
        bool prime = true;
        for (idx_t d = 2; d * d <= p; ++d) if (p % d == 0) { prime = false; break; }
        if (!prime) continue;
        idx_t e = vfact(m, p) - vfact(m - k + 1, p);
        for (idx_t i = 0; i < nm; ++i) e -= vfact(mult[i], p);
        while (e--) res *= (seq_t)p;
    }
    return res;
}

/* Recursive enumeration of partitions of `rem` with largest part ≤ `max`,
   filling parts[depth..] with the current partial partition. */
static void gen_partitions(idx_t m, idx_t rem, idx_t max,
                           int *parts, idx_t depth,
                           idx_t *cur_idx, idx_t target,
                           bool *found, seq_t *result)
{
    if (*found) return;
    if (rem == 0) {                       /* a complete partition */
        if (*cur_idx == target) {
            *result = compute_T(m, depth, parts);
            *found  = true;
        }
        (*cur_idx)++;
        return;
    }
    for (idx_t part = (rem < max ? rem : max); part >= 1; --part) {
        if (*found) return;
        parts[depth] = (int)part;
        gen_partitions(m, rem - part, part,
                       parts, depth + 1,
                       cur_idx, target, found, result);
    }
}

/* Public function requested by the benchmark. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* undefined for n==0, but not used */

    idx_t cur_idx = 1;              /* flattened index, starting at 1 */
    bool  found   = false;
    seq_t result  = 0;

    int parts[128];                 /* max parts for rows of interest (≤10000) */

    for (idx_t row = 1; !found; ++row) {
        gen_partitions(row, row, row,
                       parts, 0,
                       &cur_idx, n, &found, &result);
    }
    return result;
}
