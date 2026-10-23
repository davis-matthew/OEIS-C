/*
 * A018532  Divisors of 584.
 * https://oeis.org/A018532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 584 in ascending order
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Recursive helper to generate all divisors */
static void gen_divisors(size_t idx, seq_t cur,
                         const uint64_t *primes, const uint64_t *exps,
                         size_t nprimes, seq_t *divs, size_t *dcount)
{
    if (idx == nprimes) {
        divs[(*dcount)++] = cur;
        return;
    }
    seq_t val = 1;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        gen_divisors(idx + 1, cur * val, primes, exps,
                     nprimes, divs, dcount);
        val *= (seq_t)primes[idx];
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n < 1 */

    static seq_t divs[64];
    static size_t dcount = 0;

    if (dcount == 0) {
        /* Factor 584: 2^3 * 73^1 */
        uint64_t primes[10], exps[10];
        size_t cnt = 0;
        uint64_t m = 584;

        if (m % 2 == 0) {
            primes[cnt] = 2;
            exps[cnt] = 0;
            while (m % 2 == 0) {
                m /= 2;
                exps[cnt]++;
            }
            cnt++;
        }
        for (uint64_t p = 3; p * p <= m; p += 2) {
            if (m % p == 0) {
                primes[cnt] = p;
                exps[cnt] = 0;
                while (m % p == 0) {
                    m /= p;
                    exps[cnt]++;
                }
                cnt++;
            }
        }
        if (m > 1) {
            primes[cnt] = m;
            exps[cnt] = 1;
            cnt++;
        }

        gen_divisors(0, (seq_t)1, primes, exps, cnt, divs, &dcount);

        /* Simple insertion sort for the very small list */
        for (size_t i = 0; i < dcount; ++i) {
            for (size_t j = i + 1; j < dcount; ++j) {
                if (divs[i] > divs[j]) {
                    seq_t tmp = divs[i];
                    divs[i] = divs[j];
                    divs[j] = tmp;
                }
            }
        }
    }

    if ((size_t)n <= dcount) {
        return divs[(size_t)n - 1];
    } else {
        return (seq_t)0;  /* undefined for n > 8 */
    }
}
