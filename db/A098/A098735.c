/*
 * A098735  Numerator of sum of all matrix elements M(i,j) = i^2 + j^2 (i,j =
 *          1..n) divided by n!.
 * https://oeis.org/A098735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numerator( n*(n+1)*(2n+1) / (3*(n-1)!) )
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Minimal sieve for primes up to 20001 (enough for all n <= 10000) */
static bool primes_inited = false;
static size_t primes_count = 0;
static uint32_t primes[20005];

static void init_primes(void) {
    if (primes_inited) return;
    const size_t N = 20001;
    bool is_comp[N + 1];
    for (size_t i = 2; i <= N; ++i) is_comp[i] = false;
    for (size_t i = 2; i * i <= N; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= N; j += i) is_comp[j] = true;
        }
    }
    primes_count = 0;
    for (size_t i = 2; i <= N; ++i) {
        if (!is_comp[i]) primes[primes_count++] = (uint32_t)i;
    }
    primes_inited = true;
}

/* Add or increase exponent for a prime in the factor map */
static void add_factor(uint32_t p, uint32_t e,
                       uint32_t *primes_map, uint32_t *exps_map, size_t *cnt)
{
    for (size_t i = 0; i < *cnt; ++i) {
        if (primes_map[i] == p) {
            exps_map[i] += e;
            return;
        }
    }
    primes_map[*cnt] = p;
    exps_map[*cnt] = e;
    ++(*cnt);
}

/* Factor a number x (x <= 20001) and update the factor map */
static void factor_number(uint32_t x,
                          uint32_t *primes_map, uint32_t *exps_map, size_t *cnt)
{
    uint32_t tmp = x;
    for (size_t i = 0; i < primes_count && (uint32_t)(primes[i] * primes[i]) <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            uint32_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            add_factor(p, e, primes_map, exps_map, cnt);
        }
    }
    if (tmp > 1) { /* remaining prime factor > sqrt(x) */
        add_factor(tmp, 1, primes_map, exps_map, cnt);
    }
}

seq_t a(idx_t n) {
    init_primes();

    /* n*(n+1)*(2n+1) as the initial numerator */
    seq_t num = (seq_t)n * ((seq_t)n + 1) * ((seq_t)n * 2 + 1);

    /* Factorization map: at most a few primes for each operand */
    uint32_t factor_primes[64];
    uint32_t factor_exps[64];
    size_t factor_cnt = 0;

    factor_number((uint32_t)n, factor_primes, factor_exps, &factor_cnt);
    factor_number((uint32_t)n + 1, factor_primes, factor_exps, &factor_cnt);
    factor_number((uint32_t)(2 * n) + 1, factor_primes, factor_exps, &factor_cnt);

    /* n-1 for factorial part */
    idx_t n1 = n - 1U;

    /* For each prime in the map, cancel common powers with factorial */
    for (size_t i = 0; i < factor_cnt; ++i) {
        uint32_t p = factor_primes[i];
        uint32_t exp_num = factor_exps[i];

        /* exponent of p in (n-1)! */
        idx_t div = p;
        idx_t exp_fact = 0;
        while (div <= n1) {
            exp_fact += n1 / div;
            if (div > n1 / p) break;  /* prevent overflow */
            div *= p;
        }
        if (p == 3) exp_fact += 1;  /* extra factor 3 from denominator */

        idx_t e_common = exp_num <= exp_fact ? exp_num : exp_fact;

        if (e_common > 0) {
            seq_t powp = 1;
            for (idx_t j = 0; j < e_common; ++j) powp *= (seq_t)p;
            num = num / powp;  /* exact division guaranteed */
        }
    }

    return num;  /* numerator after reduction */
}
