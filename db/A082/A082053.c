/*
 * A082053  Sum of divisors of n that are not of the form 4k+3.
 * https://oeis.org/A082053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{d|n, d%4!=3} d (sum of all divisors of n minus sum
 *            of the divisors that are 3 mod 4)
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 8855 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void enumerate_divisors_and_sum3(const uint64_t *primes, const int *exps, int cnt,
                                        seq_t *sum3)
{
    /* There are at most 128 divisors for n <= 10000, so a small fixed array
       suffices. */
    uint64_t divs[200];
    int sz = 1;
    divs[0] = 1;

    for (int i = 0; i < cnt; ++i) {
        uint64_t new_divs[200];
        int new_sz = 0;
        uint64_t mult = 1;
        for (int j = 0; j <= exps[i]; ++j) {
            for (int k = 0; k < sz; ++k) {
                new_divs[new_sz++] = divs[k] * mult;
            }
            mult *= primes[i];
        }
        sz = new_sz;
        for (int k = 0; k < sz; ++k) divs[k] = new_divs[k];
    }

    *sum3 = 0;
    for (int i = 0; i < sz; ++i) {
        if (divs[i] % 4 == 3) *sum3 = *sum3 + (seq_t)divs[i];
    }
}

/* Computes the prime factorisation of n and the sum of its divisors */
static void factoring_and_sigma(idx_t n, uint64_t *primes, int *exps, int *cnt, seq_t *sigma)
{
    uint64_t x = n;
    int c = 0;
    for (uint64_t d = 2; d * d <= x; d += (d == 2 ? 1 : 2)) {
        if (x % d == 0) {
            int e = 0;
            while (x % d == 0) {
                x /= d;
                ++e;
            }
            primes[c] = d;
            exps[c]   = e;
            ++c;
        }
    }
    if (x > 1) {
        primes[c] = x;
        exps[c]   = 1;
        ++c;
    }

    *cnt = c;
    *sigma = 1;
    for (int i = 0; i < c; ++i) {
        seq_t term = 1;
        seq_t power = 1;
        for (int k = 0; k < exps[i]; ++k) {
            power = power * (seq_t)primes[i];
            term  = term + power;
        }
        *sigma = *sigma * term;
    }
}

seq_t a(idx_t n)
{
    /* n is at least 1 by definition of the sequence. */
    uint64_t primes[128];
    int exps[128];
    int cnt;
    seq_t sigma;

    factoring_and_sigma(n, primes, exps, &cnt, &sigma);
    seq_t sum3;
    enumerate_divisors_and_sum3(primes, exps, cnt, &sum3);
    return sigma - sum3;
}
