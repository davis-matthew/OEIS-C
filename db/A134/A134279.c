/*
 * A134279  A certain partition array in Abramowitz-Stegun order (A-St order),
 *          called M_3(6)/M_3.
 * https://oeis.org/A134279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = product of A008548(part) over the k‑th partition of * n
 *            in Abramowitz–Stegun (A‑St) order, flattened by rows of length *
 *            p(n) (the partition numbers). The function returns the n‑th term
 *            * of OEIS sequence A134279
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* --- helper to find which integer m and which partition number k
 *     correspond to the overall index n (1‑based) ------------------ */
static void find_n_pos(uint64_t idx,
                       uint64_t *target_m, uint64_t *target_k)
{
    if (idx == 0) { *target_m = 0; *target_k = 0; return; }

    uint64_t cum = 0;
    /* dynamic array to store partition counts p(0..m) */
    static uint64_t *p_arr = NULL;
    static size_t cap = 0;
    if (!p_arr) {             /* first call: allocate for p(0) */
        cap = 1;
        p_arr = (uint64_t *)malloc(cap * sizeof(uint64_t));
        if (!p_arr) exit(1);
        p_arr[0] = 1;          /* p(0) = 1 */
    }

    for (uint64_t m = 1;; ++m) {
        /* grow array if necessary */
        if (m >= cap) {
            size_t newcap = cap * 2;
            if (newcap <= m) newcap = m + 1;
            uint64_t *tmp = (uint64_t *)realloc(p_arr, newcap * sizeof(uint64_t));
            if (!tmp) exit(1);
            p_arr = tmp;
            cap = newcap;
        }

        /* compute p(m) using Euler's recurrence */
        int64_t val = 0;
        for (int k = 1; ; ++k) {
            uint64_t p1 = (uint64_t)k * ((uint64_t)3 * k - 1) / 2;
            if (p1 > m) break;
            int sign = (k & 1) ? 1 : -1;
            val += sign * (int64_t)p_arr[m - p1];
            uint64_t p2 = (uint64_t)k * ((uint64_t)3 * k + 1) / 2;
            if (p2 > m) continue;
            val += sign * (int64_t)p_arr[m - p2];
        }
        p_arr[m] = (uint64_t)val;

        if (cum + p_arr[m] >= idx) {
            *target_m = m;
            *target_k = idx - cum;
            return;
        }
        cum += p_arr[m];
    }
}

/* --- generate partitions of 'rem' into 'len' parts, parts non‑increasing,
 *     in order of decreasing largest part within each fixed length. ---- */
static void gen_by_len(int rem, int len, int maxpart, seq_t prod,
                       uint64_t *cnt, uint64_t target,
                       seq_t *out, bool *found, const seq_t *partProd)
{
    if (*found) return;
    if (len == 0) {
        if (rem == 0) {
            (*cnt)++;
            if (*cnt == target) {
                *out = prod;
                *found = true;
            }
        }
        return;
    }
    /* need at least 1 in each remaining part */
    int upper = (int)(rem - (len - 1));
    if (upper > maxpart) upper = maxpart;
    for (int k = upper; k >= 1; --k) {
        gen_by_len(rem - k, len - 1, k,
                   prod * partProd[k], cnt, target, out, found, partProd);
        if (*found) return;
    }
}

/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* out of range */

    uint64_t m, k;
    find_n_pos((uint64_t)n, &m, &k);

    /* pre‑compute 5‑factorials A008548(i) for 0<=i<=m */
    seq_t *prodFact = mem_new(seq_t, (m + 1));
    if (!prodFact) exit(1);
    prodFact[0] = (seq_t)1;
    for (uint64_t i = 1; i <= m; ++i) {
        seq_t factor = (seq_t)(5 * (i - 1) + 1);
        prodFact[i] = prodFact[i - 1] * factor;
    }

    seq_t result = (seq_t)0;
    uint64_t count = 0;
    bool found = false;

    /* enumerate by number of parts ascending */
    for (int len = 1; len <= m; ++len) {
        gen_by_len((int)m, len, (int)m, (seq_t)1,
                   &count, k, &result, &found, prodFact);
        if (found) break;
    }

    mem_free(prodFact);
    return result;
}
