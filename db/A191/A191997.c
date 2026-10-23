/*
 * A191997  Denominators of partial products of a Hardy-Littlewood constant.
 * https://oeis.org/A191997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = denominator of 2 * Π_{j=2..n} (1 - 1/(p(j)-1)^2), where
 *            p(j) is the j-th prime
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   get_prime, pow_seq, seq_mul, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include "../utils/get_prime.h"
#include "../utils/seq_one.h"
#include "../utils/seq_mul.h"
#include "../utils/pow_seq.h"

#define MAX_PRIME_EST 200000   /* safe upper bound for n ≤ 10000 */

/* small primes up to 500 used for factorisation */
static int small_primes[200];
static int small_cnt = 0;
static int small_primes_initialized = 0;

static void init_small_primes(void)
{
    if (small_primes_initialized) return;
    small_primes_initialized = 1;
    int limit = 500;
    int is_prime[501];
    for (int i = 2; i <= limit; ++i) is_prime[i] = 1;
    for (int i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= limit; j += i) is_prime[j] = 0;
        }
    }
    for (int i = 2; i <= limit; ++i) {
        if (is_prime[i]) small_primes[small_cnt++] = i;
    }
}

/* array holding the net exponent difference (denominator exponent – numerator exponent) */
static int32_t diff[MAX_PRIME_EST + 1];
static int diff_initialized = 0;

/* add the factorization of x to diff with a given sign:
   sign = +1  adds exponent,
   sign = -1  subtracts exponent.
   For (p-1)^2 we call with sign = +2. */
static void add_factorization(uint64_t x, int sign)
{
    uint64_t v = x;
    for (int k = 0; k < small_cnt; ++k) {
        int p = small_primes[k];
        if ((uint64_t)p * p > v) break;
        int cnt = 0;
        while (v % p == 0) {
            v /= p;
            ++cnt;
        }
        if (cnt) diff[p] += sign * cnt;
    }
    if (v > 1 && v <= (uint64_t)MAX_PRIME_EST) {
        diff[(int)v] += sign * 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* not used, but harmless */
    if (n == 1) return seq_one();        /* denominator of r(1)=2 is 1 */

    init_small_primes();

    /* reset diff up to the needed range */
    uint32_t prime_n = get_prime(n);
    int limit = (int)prime_n + 5;
    if (limit > MAX_PRIME_EST) limit = MAX_PRIME_EST;
    memset(diff, 0, (limit + 1) * sizeof(int32_t));

    /* initial numerator factor 2 */
    if (2 <= limit) diff[2] -= 1;

    /* process primes p_2 .. p_n */
    for (idx_t i = 2; i <= n; ++i) {
        uint32_t p = get_prime(i);
        add_factorization((uint64_t)(p - 1), +2);   /* (p-1)^2 in denominator */
        add_factorization((uint64_t)p,     -1);     /* p in numerator   */
        if (p > 2) {                               /* p-2 is ≥1, skip 1 */
            add_factorization((uint64_t)(p - 2), -1);  /* p-2 in numerator */
        }
    }

    /* build the denominator from the remaining positive exponents */
    seq_t den = seq_one();
    for (int p = 2; p <= limit; ++p) {
        int e = diff[p];
        if (e > 0) {
            seq_t pe = pow_seq((seq_t)p, e);
            den = seq_mul(den, pe);
        }
    }
    return den;
}
