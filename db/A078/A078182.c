/*
 * A078182  a(n) = Sum_{d|n, d == 2 (mod 3)} d.
 * https://oeis.org/A078182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{d|n, d == 2 (mod 3)} d
 * Range:     u8 n <= 199, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Pre‑compute small primes for factorisation. */
#define PRIMELIMIT 100000

static idx_t primes[PRIMELIMIT];
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (prime_cnt != 0) return;            /* already initialised */
    static bool is_prime[PRIMELIMIT + 1];
    for (idx_t i = 2; i <= PRIMELIMIT; ++i) is_prime[i] = true;
    for (idx_t i = 2; i * i <= PRIMELIMIT; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= PRIMELIMIT; j += i) is_prime[j] = false;
    for (idx_t i = 2; i <= PRIMELIMIT; ++i)
        if (is_prime[i]) primes[prime_cnt++] = i;
}

/* Compute a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of range – return 0 */

    init_primes();

    /* Factorise n.  Use at most 16 prime factors for n ≤ 10⁴. */
    idx_t pf[16];
    int exp[16];
    size_t pf_cnt = 0;
    idx_t tmp = n;
    for (size_t pi = 0; pi < prime_cnt && primes[pi] * primes[pi] <= tmp; ++pi) {
        if (tmp % primes[pi] == 0) {
            int e = 0;
            while (tmp % primes[pi] == 0) {
                tmp /= primes[pi];
                ++e;
            }
            pf[pf_cnt] = primes[pi];
            exp[pf_cnt] = e;
            ++pf_cnt;
        }
    }
    if (tmp > 1) {                        /* remaining prime factor */
        pf[pf_cnt] = tmp;
        exp[pf_cnt] = 1;
        ++pf_cnt;
    }

    /* Generate all divisors iteratively.  Store them in a temporary array. */
    seq_t divs[256];
    size_t cnt = 1;
    divs[0] = (seq_t)1;

    for (size_t i = 0; i < pf_cnt; ++i) {
        size_t old_cnt = cnt;
        seq_t p = pf[i];
        seq_t p_pow = 1;
        for (int k = 1; k <= exp[i]; ++k) {
            p_pow = p_pow * p;                 /* p^k */
            for (size_t j = 0; j < old_cnt; ++j)
                divs[cnt++] = divs[j] * p_pow;
        }
    }

    /* Sum those divisors that are ≡ 2 (mod 3). */
    seq_t result = 0;
    for (size_t i = 0; i < cnt; ++i)
        if (divs[i] % (seq_t)3 == (seq_t)2)
            result += divs[i];

    return result;
}
