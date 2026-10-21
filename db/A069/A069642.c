/*
 * A069642  Let M_n be the n X n matrix with M_n(i,j)=1/(i+j+3); then
 *          a(n)=1/det(M_n).
 * https://oeis.org/A069642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Π_{m=5}^{2n+3} m^{c(m)} / Π_{k=1}^{n-1} (k!)² where c(m)
 *            = (m-3 ≤ n+1) ? (m-4) : (2n+4 – m) . This follows from the
 *            Cauchy determinant for 1/(x_i+x_j) with x_i=i+3/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   build_primes, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/build_primes.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;   /* undefined, keep consistent */
    }

    idx_t limit = 2*n + 3;          /* upper bound for m and primes */
    idx_t N = n - 1;                /* max factorial index */

    /* Build list of primes up to limit. */
    idx_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes(limit, &primes, &prime_cnt);

    /* Prime index lookup table: prime → position in primes[] */
    int32_t *prime_idx = (int32_t*)calloc((size_t)(limit + 1), sizeof(int32_t));
    for (size_t i = 0; i < prime_cnt; ++i)
        prime_idx[primes[i]] = (int32_t)i;

    /* Exponent accumulator for each prime. */
    int64_t *exps = (int64_t*)calloc(prime_cnt, sizeof(int64_t));

    /* ---------- Numerator part ---------- */
    for (idx_t m = 5; m <= limit; ++m) {
        idx_t t = m - 3;
        idx_t c = (t <= n + 1) ? (t - 1) : (2*n + 1 - t);

        idx_t val = m;
        for (size_t pi = 0; pi < prime_cnt && (idx_t)(primes[pi] * primes[pi]) <= val; ++pi) {
            idx_t p = primes[pi];
            if (val % p == 0) {
                int64_t cnt = 0;
                while (val % p == 0) {
                    val /= p;
                    ++cnt;
                }
                exps[pi] += c * cnt;
            }
        }
        if (val > 1) {   /* remaining prime factor */
            int32_t pi = prime_idx[val];
            exps[pi] += c;
        }
    }

    /* ---------- Denominator part (product of (k!)²) ---------- */
    for (idx_t j = 1; j <= N; ++j) {
        idx_t val = j;
        idx_t mult = N - j + 1;   /* j appears in mult factorials */

        for (size_t pi = 0; pi < prime_cnt && (idx_t)(primes[pi] * primes[pi]) <= val; ++pi) {
            idx_t p = primes[pi];
            if (val % p == 0) {
                int64_t cnt = 0;
                while (val % p == 0) {
                    val /= p;
                    ++cnt;
                }
                exps[pi] -= 2 * cnt * mult;
            }
        }
        if (val > 1) {
            int32_t pi = prime_idx[val];
            exps[pi] -= 2 * mult;
        }
    }

    /* ---------- Assemble result ---------- */
    seq_t result = (seq_t)1;
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        int64_t e = exps[pi];
        if (e > 0) {
            result = seq_pow((seq_t)primes[pi], (idx_t)e) * result;
        }
    }

    /* Clean up */
    free(primes);
    free(prime_idx);
    free(exps);

    return result;
}
