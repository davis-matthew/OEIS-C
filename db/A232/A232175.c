/*
 * A232175  Least positive k such that n^3 + k^2 is a square, or 0 if there is
 *          no such k.
 * https://oeis.org/A232175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Least positive k such that n^3 + k^2 is a square,
 *            returning 0 if no such k exists
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1979, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper to generate all divisors of n^3 from its prime factorisation. */
static void gen_divs(idx_t idx, seq_t cur,
                     const idx_t *primes, const idx_t *exps, idx_t fcnt,
                     seq_t *out, idx_t *cnt)
{
    if (idx == fcnt) {
        out[*cnt] = cur;
        (*cnt)++;
        return;
    }
    seq_t val = cur;
    for (idx_t e = 0; e <= exps[idx]; ++e) {
        gen_divs(idx + 1, val, primes, exps, fcnt, out, cnt);
        val = val * (seq_t)primes[idx];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* work only for n >= 1 */

    /* 1. Prime factorise n, collecting primes and exponents of n^3. */
    idx_t tmp = n;
    const idx_t MAX_FACTS = 20;
    idx_t primes[MAX_FACTS];
    idx_t exps[MAX_FACTS];
    idx_t fcnt = 0;

    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            idx_t cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
            }
            primes[fcnt] = p;
            exps[fcnt] = cnt * 3;   /* exponent in n^3 */
            ++fcnt;
        }
    }
    if (tmp > 1) {
        primes[fcnt] = tmp;
        exps[fcnt]   = 3;
        ++fcnt;
    }

    /* 2. Compute n^3 (safe in seq_t for n <= 10000). */
    seq_t n3 = (seq_t)n * n * n;

    /* 3. Generate all divisors of n^3. */
    const idx_t MAX_DIVS = 2000;   /* ample for n <= 10000 */
    seq_t divs[MAX_DIVS];
    idx_t dcnt = 0;
    gen_divs(0, 1, primes, exps, fcnt, divs, &dcnt);

    /* 4. Find minimal positive k. */
    seq_t best = 0;   /* 0 indicates no candidate yet */
    for (idx_t i = 0; i < dcnt; ++i) {
        seq_t u = divs[i];
        seq_t v = n3 / u;   /* v = n^3 / u */
        if (u > v) continue;             /* ensure u <= v */
        if (((u + v) & 1) != 0) continue; /* need same parity for m */
        seq_t k = (v - u) / 2;
        if (k == 0) continue;             /* need positive k */
        if (best == 0 || k < best) best = k;
    }
    return best;
}
