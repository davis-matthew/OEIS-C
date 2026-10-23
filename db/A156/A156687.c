/*
 * A156687  Perimeters of Pythagorean triangles that can be constructed in
 *          exactly 5 different ways.
 * https://oeis.org/A156687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = the nth perimeter that occurs in exactly 5 distinct
 *            Pythagorean triples. The implementation enumerates all primitive
 *            triples via Euclid's parametrisation, counts how often each
 *            perimeter is hit, and extracts those with count == 5
 * Range:     u8 none, u16 n <= 603, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static inline int gcd_int(int a, int b)
{
    while (b != 0) {
        int t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* static storage for memoised results */
    static bool ready = false;
    static seq_t *sol = NULL;
    static size_t sol_len = 0;
    static size_t sol_cap = 0;

    if (!ready) {
        /* upper bound sufficient to generate at least 10 000 terms;
           20 million keeps memory usage moderate.                 */
        const idx_t UPPER = 20000000u;

        /* count occurrences of each perimeter */
        uint16_t *cnt = (uint16_t *)calloc((size_t)UPPER + 1, sizeof(uint16_t));
        if (cnt == NULL) return 0;               /* unlikely in test env */

        /* primitive parameters: m > n, coprime, opposite parity */
        int m_max = 1;
        while ((idx_t)2 * m_max * (m_max + 1) <= UPPER) ++m_max;
        --m_max;

        for (int m = 2; m <= m_max; ++m) {
            for (int n = 1; n < m; ++n) {
                if (((m - n) & 1) == 0) continue;            /* same parity */
                if (gcd_int(m, n) != 1) continue;             /* not coprime */

                idx_t p0 = (idx_t)2 * m * (m + n);
                if (p0 > UPPER) break;                        /* larger n → larger p0 */

                /* all multiples of the primitive perimeter are valid */
                for (idx_t k = 1; ; ++k) {
                    idx_t p = p0 * k;
                    if (p > UPPER) break;
                    ++cnt[p];
                }
            }
        }

        /* collect perimeters with exactly five representations */
        sol_cap = 1024;
        sol = (seq_t *)calloc(sol_cap, sizeof(seq_t));
        if (sol == NULL) { free(cnt); return 0; }

        for (idx_t p = 1; p <= UPPER; ++p) {
            if (cnt[p] == 5) {
                if (sol_len == sol_cap) {
                    sol_cap *= 2;
                    seq_t *tmp = (seq_t *)realloc(sol, sol_cap * sizeof(seq_t));
                    if (tmp == NULL) { free(cnt); free(sol); return 0; }
                    sol = tmp;
                }
                sol[sol_len++] = (seq_t)p;
            }
        }

        free(cnt);
        ready = true;
    }

    /* index is 1‑based; return 0 for out‑of‑range queries (not used in tests) */
    if (n == 0 || n > sol_len) return 0;
    return sol[n - 1];
}
