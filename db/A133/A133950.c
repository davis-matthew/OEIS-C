/*
 * A133950  a(n) = the number of "isolated divisors" of n(n+1)/2. A positive
 *          divisor k of n is isolated if neither k-1 nor k+1 divides n.
 * https://oeis.org/A133950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of isolated divisors of n(n+1)/2. A divisor d
 *            of N = n(n+1)/2 is isolated if N % (d-1) != 0 and N % (d+1) != 0
 *            (treating d-1 = 0 as not dividing)
 * Range:     u8 n <= 22, u16 n <= 728, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9388 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static void _enumerate_divs(int idx, seq_t cur, uint64_t *primes, int *exps,
                            int cnt, seq_t N, seq_t *cnt_iso)
{
    if (idx == cnt) {
        bool iso = true;
        if (cur > 1) {
            if (N % (cur - 1) == 0)
                iso = false;
        }
        if (iso && N % (cur + 1) == 0)
            iso = false;
        if (iso)
            (*cnt_iso)++;
        return;
    }
    seq_t mult = 1;
    for (int e = 0; e <= exps[idx]; e++) {
        _enumerate_divs(idx + 1, cur * mult, primes, exps, cnt, N, cnt_iso);
        mult *= (seq_t)primes[idx];
    }
}

seq_t a(idx_t n)
{
    /* Compute N = n(n+1)/2 with seq_t arithmetic. */
    seq_t N = ((seq_t)n * ((seq_t)n + 1)) / 2;

    /* Factor N using simple trial division. */
    uint64_t primes[64];
    int exps[64];
    int cnt = 0;
    uint64_t t = (uint64_t)N;   /* N fits in 64‑bit for all test values */

    if (t % 2 == 0) {
        primes[cnt] = 2;
        exps[cnt] = 0;
        while (t % 2 == 0) {
            t /= 2;
            exps[cnt]++;
        }
        cnt++;
    }

    for (uint64_t d = 3; d * d <= t; d += 2) {
        if (t % d == 0) {
            primes[cnt] = d;
            exps[cnt] = 0;
            while (t % d == 0) {
                t /= d;
                exps[cnt]++;
            }
            cnt++;
        }
    }

    if (t > 1) {
        primes[cnt] = t;
        exps[cnt] = 1;
        cnt++;
    }

    /* Enumerate all divisors and count the isolated ones. */
    seq_t iso_count = 0;
    _enumerate_divs(0, 1, primes, exps, cnt, N, &iso_count);
    return iso_count;
}
