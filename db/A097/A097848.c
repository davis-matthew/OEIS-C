/*
 * A097848  Triangle read by rows: T(n,k) = minimal number of terms needed to
 *          write k/n (for 1 <= k <= n with gcd(k,n) = 1) as a sum of unit
 *          fractions.
 * https://oeis.org/A097848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   ceil_div_uint64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A097848: Minimal number of unit fractions for k/n
   Triangle indexed by rows, cumulative over reduced fractions.
   The value a(n) is obtained by locating the (k,n) pair corresponding
   to the n‑th element in the triangle read by rows and computing the
   minimal Egyptian‑fraction length for k/n using a small‑depth search
   that improves on the greedy algorithm. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/ceil_div_uint64.h"

/* ----------  Helpers ---------- */

/* Greedy Egyptian‑fraction length (upper bound). */
static seq_t greedy_len(uint64_t num, uint64_t den)
{
    seq_t count = 0;
    seq_t a = (seq_t)num;
    seq_t b = (seq_t)den;

    while (a != 0) {
        seq_t d = (b + a - 1) / a;
        seq_t na = a * d - b;
        seq_t nb = b * d;
        seq_t g = gcd(na, nb);
        if (g != 0) {
            na /= g;
            nb /= g;
        }
        a = na;
        b = nb;
        ++count;
    }
    return count;
}

/* Minimal length search with limited depth (returns UINT8_MAX if no solution found within depth). */
static seq_t min_len_rec(uint64_t a, uint64_t b, uint8_t depth_limit)
{
    if (depth_limit == 0)          /* no more depth left */
        return UINT8_MAX;
    if (a == 0)
        return 0;
    if (b % a == 0)                /* can finish in one term */
        return 1;
    if (depth_limit <= 1)          /* cannot recurse further */
        return UINT8_MAX;

    uint64_t d_start = ceil_div_uint64(b, a);
    seq_t best = UINT8_MAX;
    for (uint64_t d = d_start; d <= d_start + depth_limit - 1; ++d) {
        uint64_t na = a * d - b;
        uint64_t nb = b * d;
        if (na == 0) {
            best = 1;
            break;
        }
        uint64_t g = (uint64_t)gcd((seq_t)na, (seq_t)nb);
        if (g != 0) {
            na /= g;
            nb /= g;
        }
        seq_t sub = min_len_rec(na, nb, depth_limit - 1);
        if (sub != UINT8_MAX) {
            seq_t len = sub + 1;
            if (len < best)
                best = len;
            if (best == 2)                /* cannot get better than 2 */
                break;
        }
    }
    return best;
}

static seq_t min_len(uint64_t a, uint64_t b)
{
    /* Upper bound from greedy */
    seq_t ub = greedy_len(a, b);
    /* Search for better using recursive depth ≤ ub */
    seq_t best = min_len_rec(a, b, (uint8_t)ub);
    return best == UINT8_MAX ? ub : best;
}

/* ----------  Row/Coprime mapping ---------- */

/* Memoised phi and prefix sums */
static idx_t *phi = NULL;
static idx_t *cum = NULL;
static idx_t phi_cap = 0;

static void ensure_phi_upto(idx_t n)
{
    if (n < 1)
        return;
    if (n + 1 <= phi_cap)
        return;

    idx_t new_cap = n + 1;
    idx_t *new_phi = (idx_t*)malloc(sizeof(idx_t) * new_cap);
    idx_t *new_cum = (idx_t*)malloc(sizeof(idx_t) * new_cap);

    for (idx_t i = 0; i < new_cap; ++i)
        new_phi[i] = i;

    for (idx_t i = 2; i < new_cap; ++i) {
        if (new_phi[i] == i) {                 /* i is prime */
            for (idx_t j = i; j < new_cap; j += i)
                new_phi[j] -= new_phi[j] / i;
        }
    }

    new_cum[0] = 0;
    for (idx_t i = 1; i < new_cap; ++i)
        new_cum[i] = new_cum[i - 1] + new_phi[i];

    free(phi);
    free(cum);
    phi = new_phi;
    cum = new_cum;
    phi_cap = new_cap;
}

/* ----------  Main sequence function ---------- */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* outside offset – defined value */

    ensure_phi_upto(n);

    /* Binary search for row r such that cum[r‑1] < n ≤ cum[r] */
    idx_t low = 1, high = phi_cap - 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (cum[mid] >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t row = low;
    idx_t prev = cum[row - 1];
    idx_t pos = n - prev;                /* 1‑based position within the row */

    /* Find pos‑th coprime integer k for this row */
    idx_t count = 0;
    idx_t k_val = 0;
    for (idx_t k = 1; k <= row; ++k) {
        if (gcd((seq_t)k, (seq_t)row) == 1) {
            ++count;
            if (count == pos) {
                k_val = k;
                break;
            }
        }
    }

    /* Compute minimal Egyptian‑fraction length for k_val / row */
    return min_len((uint64_t)k_val, (uint64_t)row);
}
