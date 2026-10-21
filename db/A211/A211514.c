/*
 * A211514  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          3*w^2=x*y.
 * https://oeis.org/A211514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count ordered triples (w,x,y) with w,x,y in {1..n} and 3*w^2 = x*y */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ----------  Sieve of smallest prime factors  ---------- */
static idx_t spf_limit = 0;
static uint32_t *spf_arr   = NULL;   /* spf_arr[i] = smallest prime factor of i */
static uint32_t *prime_list = NULL;  /* list of primes up to spf_limit */
static size_t   prime_count = 0;

static void init_sieve(idx_t limit)
{
    if (limit <= spf_limit) return;

    size_t sz = (size_t)(limit + 1);
    uint32_t *new_spf = (uint32_t*)malloc(sz * sizeof(*new_spf));
    if (!new_spf) return;               /* allocation failure – leave empty */
    for (size_t i = 0; i < sz; ++i) new_spf[i] = 0;
    new_spf[0] = 0; new_spf[1] = 1;

    for (size_t i = 2; i < sz; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = (uint32_t)i;
            if (i * i <= limit) {
                for (size_t j = i * i; j < sz; j += i)
                    if (new_spf[j] == 0) new_spf[j] = (uint32_t)i;
            }
        }
    }

    /* collect primes */
    size_t cnt = 0;
    for (size_t i = 2; i < sz; ++i)
        if (new_spf[i] == (uint32_t)i) ++cnt;

    uint32_t *new_primes = (uint32_t*)malloc(cnt * sizeof(*new_primes));
    if (!new_primes) {
        free(new_spf);
        return;
    }
    size_t idx = 0;
    for (size_t i = 2; i < sz; ++i)
        if (new_spf[i] == (uint32_t)i)
            new_primes[idx++] = (uint32_t)i;

    if (spf_arr) free(spf_arr);
    if (prime_list) free(prime_list);
    spf_arr = new_spf;
    prime_list = new_primes;
    prime_count = cnt;
    spf_limit = limit;
}

/* ----------  Enumerate divisors of M = 3*w^2  ---------- */
static void count_divisors(const uint32_t *pr, const int *exp, size_t dim, size_t idx,
                           seq_t cur, seq_t M, seq_t n_seq, seq_t *sum)
{
    if (idx == dim) {
        seq_t d = cur;
        seq_t e = M / d;
        if (d <= e) {
            if (d <= n_seq && e <= n_seq) {
                if (d == e) *sum += (seq_t)1;
                else        *sum += (seq_t)2;
            }
        }
        return;
    }
    seq_t p = (seq_t)pr[idx];
    seq_t pow = (seq_t)1;
    for (int i = 0; i <= exp[idx]; ++i) {
        count_divisors(pr, exp, dim, idx + 1, cur * pow, M, n_seq, sum);
        pow *= p;
    }
}

/* ----------  Main sequence function  ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    init_sieve(n);          /* factorisation table for 1..n */

    seq_t total = (seq_t)0;
    seq_t n_seq = (seq_t)n;

    for (idx_t w = 1; w <= n; ++w) {
        /* factorise w */
        uint32_t primes_local[32];
        int      exps_local[32];
        size_t   dim = 0;

        idx_t tmp = w;
        while (tmp > 1) {
            uint32_t p = spf_arr[(size_t)tmp];
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes_local[dim] = p;
            exps_local[dim] = 2 * e;   /* exponent in w^2 */
            ++dim;
        }

        /* add the extra factor 3 for 3*w^2 */
        bool has3 = false;
        for (size_t i = 0; i < dim; ++i) {
            if (primes_local[i] == 3) {
                exps_local[i] += 1;
                has3 = true;
                break;
            }
        }
        if (!has3) {
            primes_local[dim] = 3;
            exps_local[dim] = 1;
            ++dim;
        }

        seq_t M = (seq_t)w * (seq_t)w * (seq_t)3;
        count_divisors(primes_local, exps_local, dim, 0, (seq_t)1, M, n_seq, &total);
    }

    return total;
}
