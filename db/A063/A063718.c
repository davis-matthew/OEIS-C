/*
 * A063718  a(n) is the smallest divisor of n^2 that is greater than n.
 * https://oeis.org/A063718
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the smallest divisor of n² that is greater than n by generating all
 * divisors of n² and selecting the minimum value exceeding n. */
#include <stdint.h>
#include <stddef.h>

/* Generate all divisors of n². Since n ≤ 10 000 in the tests, the number of primes
 * involved is at most 5 and the number of divisors of n² is < 200.  We keep all
 * data in static buffers to avoid dynamic allocation. */
#define MAX_PRIMES 10
#define MAX_DIVISORS 200

static void gen_divisors(idx_t idx, seq_t cur,
                         const idx_t *fprimes, const idx_t *fe,
                         size_t fp, seq_t *div, size_t *divcnt)
{
    if (idx == fp) {
        div[(*divcnt)++] = cur;
        return;
    }
    idx_t p = fprimes[idx];
    idx_t exp = fe[idx] * 2;            /* exponent range 0 .. 2*e_i */
    seq_t tmp = cur;
    for (idx_t e = 0; e <= exp; ++e) {
        gen_divisors(idx + 1, tmp, fprimes, fe, fp, div, divcnt);
        tmp *= (seq_t)p;
    }
}

seq_t a(idx_t n)
{
    /* factor n */
    idx_t fprimes[MAX_PRIMES];
    idx_t fe[MAX_PRIMES];
    size_t fp = 0;
    idx_t m = n;

    /* trial division */
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            size_t cnt = 0;
            while (m % p == 0) {
                m /= p;
                cnt++;
            }
            fprimes[fp] = p;
            fe[fp] = cnt;
            fp++;
        }
    }
    if (m > 1) {                 /* remaining prime factor */
        fprimes[fp] = m;
        fe[fp] = 1;
        fp++;
    }

    /* build divisors of n² */
    seq_t divs[MAX_DIVISORS];
    size_t divcnt = 0;
    gen_divisors(0, (seq_t)1, fprimes, fe, fp, divs, &divcnt);

    /* find smallest divisor > n */
    seq_t best = 0;
    seq_t sn = (seq_t)n;
    for (size_t i = 0; i < divcnt; ++i) {
        seq_t d = divs[i];
        if (d > sn) {
            if (best == 0 || d < best)
                best = d;
        }
    }
    return best;
}
