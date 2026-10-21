/*
 * A214845  Triangle read by rows: T(n,m) =(n/k)^(k-1) mod k, where k is the
 *          m-th divisor of n, 1 <= m <= tau(n).
 * https://oeis.org/A214845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, less
 * Formula:   a(n) = the nth element of the triangle read by rows, where each
 *            triangle entry T(n,k) = (n/k)^(k-1) mod k with k the m‑th
 *            divisor of n
 * Range:     u8 n <= 111, u16 n <= 111, u32 n <= 111, u64 n <= 111,
 *            u128 n <= 111, big any n
 * Helpers:   factor_u64, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/mod_pow.h"

static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

static void gen_divisors(uint64_t cur, size_t idx, const uint64_t *pr,
                         const uint64_t *ex, size_t npr,
                         uint64_t *out, size_t *pos)
{
    if (idx == npr) {
        out[(*pos)++] = cur;
        return;
    }
    uint64_t val = cur;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        gen_divisors(val, idx + 1, pr, ex, npr, out, pos);
        val *= pr[idx];
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    uint64_t target = (uint64_t)n;
    uint64_t cumulative = 0;
    uint64_t row = 0;

    /* Find the row that contains the nth element. */
    for (;;) {
        row++;
        size_t cntFactors = 0;
        uint64_t pr[128], ex[128];
        factor_u64(row, pr, ex, &cntFactors);

        uint64_t tau = 1;
        for (size_t i = 0; i < cntFactors; ++i) tau *= (ex[i] + 1);

        if (cumulative + tau >= target) break;
        cumulative += tau;
    }

    /* Position within the row's divisors (0‑based). */
    uint64_t pos_in_row = target - cumulative - 1;

    /* Regenerate primes exponents for the row to obtain all divisors. */
    size_t cntFactors = 0;
    uint64_t pr[128], ex[128];
    factor_u64(row, pr, ex, &cntFactors);

    uint64_t tau = 1;
    for (size_t i = 0; i < cntFactors; ++i) tau *= (ex[i] + 1);

    uint64_t *divs = (uint64_t *)malloc(tau * sizeof(uint64_t));
    size_t pos = 0;
    gen_divisors(1, 0, pr, ex, cntFactors, divs, &pos);
    qsort(divs, tau, sizeof(uint64_t), cmp_uint64);

    uint64_t k = divs[pos_in_row];
    seq_t result = mod_pow((seq_t)(row / k), (uint64_t)(k - 1), (seq_t)k);

    free(divs);
    return result;
}
