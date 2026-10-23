/*
 * A193337  Sum of odd divisors of sigma(n).
 * https://oeis.org/A193337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of odd divisors of sigma(n)
 * Range:     u8 n <= 15, u16 n <= 513, u32 n <= 9562, u64 n <= 9562,
 *            u128 n <= 9562, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include <string.h>

/* --- helper: integer power for seq_t ----------------------------------- */
static seq_t pow_seq_uint(seq_t base, unsigned exp)
{
    seq_t res = (seq_t)1;
    while (exp--) res *= base;
    return res;
}

/* --- simple prime sieve up to a fixed maximum --------------------------- */
#define MAX_PRIME 20000      /* sufficient for all required factorizations */
static int _primes[MAX_PRIME / 2];
static int _prime_cnt = 0;
static int _primes_built = 0;

static void _build_primes_upto(int limit)
{
    if (_primes_built && _primes[_prime_cnt - 1] >= limit) return;

    int sieve[limit + 1];
    memset(sieve, 0, sizeof(sieve));

    for (int i = 2; i <= limit; ++i) {
        if (!sieve[i]) {
            if (_prime_cnt < MAX_PRIME / 2)
                _primes[_prime_cnt++] = i;
            for (long long j = 1LL * i * i; j <= limit; j += i)
                sieve[(int)j] = 1;
        }
    }
    _primes_built = 1;
}

static void _ensure_primes(idx_t limit)
{
    int l = (int)limit;
    if (!_primes_built || _primes[_prime_cnt - 1] < l)
        _build_primes_upto(l);
}

/* --- factorization of 64‑bit integers ----------------------------------- */
static void _factor_u64(uint64_t n, uint64_t *primes_out,
                        unsigned *exps_out, int *count)
{
    *count = 0;
    idx_t lim = (idx_t)sqrt((double)n) + 1;
    _ensure_primes(lim);

    for (int i = 0; i < _prime_cnt && (uint64_t)_primes[i] *
                                      (uint64_t)_primes[i] <= n; ++i) {
        uint64_t p = _primes[i];
        if (n % p == 0) {
            unsigned e = 0;
            while (n % p == 0) { n /= p; ++e; }
            primes_out[(*count)++] = p;
            exps_out[(*count) - 1] = e;
        }
    }
    if (n > 1) {
        primes_out[(*count)++] = n;
        exps_out[(*count) - 1] = 1;
    }
}

/* --- sigma of a prime power ------------------------------------------- */
static seq_t _sigma_prime_power(uint64_t p, unsigned e)
{
    seq_t p_seq = (seq_t)p;
    seq_t num   = pow_seq_uint(p_seq, (unsigned)(e + 1));
    return (num - (seq_t)1) / (p_seq - (seq_t)1);
}

/* --- main sequence function ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* undefined for n=0 */

    /* factor n */
    uint64_t pn[64];
    unsigned en[64];
    int cntn;
    _factor_u64((uint64_t)n, pn, en, &cntn);

    /* compute sigma(n) */
    seq_t sigma = (seq_t)1;
    for (int i = 0; i < cntn; ++i)
        sigma *= _sigma_prime_power(pn[i], en[i]);

    /* extract odd part of sigma */
    seq_t s = sigma;
    while ((s & (seq_t)1) == (seq_t)0)
        s >>= 1;        /* divide by 2 */

    /* factor the odd part */
    uint64_t ps[64];
    unsigned es[64];
    int cnts;
    _factor_u64((uint64_t)s, ps, es, &cnts);

    /* compute sigma of the odd part (sum of its odd divisors) */
    seq_t result = (seq_t)1;
    for (int i = 0; i < cnts; ++i)
        result *= _sigma_prime_power(ps[i], es[i]);

    return result;
}
