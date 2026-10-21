/*
 * A086669  a(n) = number of divisors of n that are fundamental discriminants.
 * https://oeis.org/A086669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of n that are fundamental
 *            discriminants (OEIS A086669). A fundamental discriminant is D =
 *            1 or • D ≡ 1 (mod 4) and square‑free, • D ≡ 0 (mod 4), D/4 is
 *            square‑free and (D/4) ≡ 2 or 3 (mod 4)
 * Range:     u8 n <= 9867, u16 n <= 9867, u32 n <= 9867, u64 n <= 9867,
 *            u128 n <= 9867, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* ----------------------------------------------------------------
   Recursively enumerate all divisors of n from its prime factorisation
   and count those that are fundamental discriminants.
   ---------------------------------------------------------------- */
static void rec_divs(const uint64_t *pf, const int *exps, int pf_cnt,
                     int idx, seq_t cur, int e2_sel, int *sel_exps, seq_t *cnt)
{
    if (idx == pf_cnt) {                 /* reached a divisor */
        if (cur == 1) {                  /* D = 1 */
            ++*cnt;
            return;
        }

        int mod4 = (int)(cur % 4);
        if (mod4 == 1) {                 /* odd, D ≡ 1 (mod 4) */
            bool squarefree = true;
            for (int j = 0; j < pf_cnt; ++j) {
                if (sel_exps[j] > 1) { squarefree = false; break; }
            }
            if (squarefree) ++*cnt;
        } else if (mod4 == 0) {           /* even, D ≡ 0 (mod 4) */
            /* D must have exactly 2 or 3 twos (D/4 square‑free) */
            if (e2_sel >= 2 && e2_sel <= 3) {
                bool ok = true;
                for (int j = 0; j < pf_cnt; ++j) {
                    if (pf[j] == 2) continue;   /* twos handled above */
                    if (sel_exps[j] > 1) { ok = false; break; }
                }
                if (ok) {
                    seq_t m = cur / 4;          /* D/4 */
                    int m_mod4 = (int)(m % 4);
                    if (m_mod4 == 2 || m_mod4 == 3) ++*cnt;
                }
            }
        }
        return;
    }

    /* iterate over all possible exponents for prime pf[idx] */
    uint64_t p = pf[idx];
    int maxe = exps[idx];
    seq_t mult = 1;                       /* p^e */
    for (int e = 0; e <= maxe; ++e) {
        sel_exps[idx] = e;
        seq_t next_cur = cur * mult;
        int next_e2 = e2_sel;
        if (p == 2) next_e2 = e;          /* record exponent of 2 */
        rec_divs(pf, exps, pf_cnt, idx + 1, next_cur, next_e2,
                 sel_exps, cnt);
        mult *= p;
    }
}

/* ----------------------------------------------------------------
   Main sequence function.
   ---------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    uint64_t m = (uint64_t)n;            /* work in 64‑bit for factorisation */

    /* prime factorisation: store primes and exponents */
    uint64_t pf[64];
    int exps[64];
    int pf_cnt = 0;

    /* factor 2 */
    int e = 0;
    while ((m & 1) == 0) {               /* m % 2 == 0 */
        ++e;
        m >>= 1;
    }
    if (e > 0) { pf[pf_cnt] = 2ull; exps[pf_cnt] = e; ++pf_cnt; }

    /* odd primes */
    for (uint64_t d = 3; d <= m / d; d += 2) {
        if (m % d == 0) {
            e = 0;
            do { ++e; m /= d; } while (m % d == 0);
            pf[pf_cnt] = d; exps[pf_cnt] = e; ++pf_cnt;
        }
    }
    if (m > 1) {                         /* remaining prime */
        pf[pf_cnt] = m; exps[pf_cnt] = 1; ++pf_cnt;
    }

    /* array to hold chosen exponents during recursion */
    int sel_exps[64] = {0};

    seq_t cnt = 0;
    rec_divs(pf, exps, pf_cnt, 0, (seq_t)1, 0, sel_exps, &cnt);

    return cnt;
}
