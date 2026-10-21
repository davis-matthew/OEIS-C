/*
 * A051532  The abelian orders (or abelian numbers): numbers m such that every
 *          group of order m is abelian.
 * https://oeis.org/A051532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   numbers m such that m is cube‑free and for all distinct primes
 *            p,q|m, q does NOT divide p^k−1 for k=1..vp(m). Equivalent to:
 *            each prime has exponent ≤2 and no prime divides (other‑prime−1)
 *            or (other‑prime+1) when the other has exponent 2. These are
 *            precisely the abelian orders
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define PRIMELIMIT 200000  /* sufficient for n up to ~1e4 */

/* prime table */
static int primes[PRIMELIMIT];
static int prime_cnt = 0;
static bool primes_ready = false;

/* initialise primes up to PRIMELIMIT */
static void init_primes(void)
{
    if (primes_ready) return;
    bool *is_prime = (bool*)calloc(PRIMELIMIT + 1, sizeof(bool));
    for (int i = 2; i <= PRIMELIMIT; ++i) is_prime[i] = true;
    for (int p = 2; p * p <= PRIMELIMIT; ++p) if (is_prime[p]) {
        for (int q = p * p; q <= PRIMELIMIT; q += p) is_prime[q] = false;
    }
    for (int i = 2; i <= PRIMELIMIT; ++i) if (is_prime[i]) primes[prime_cnt++] = i;
    free(is_prime);
    primes_ready = true;
}

/* helper: check whether candidate satisfies the abelian order conditions */
static bool is_abelian(uint64_t m)
{
    uint64_t tmp = m;
    /* factorisation, store primes and exponents */
    uint64_t fac[12];
    int exp[12];
    int pcnt = 0;

    for (int i = 0; i < prime_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= tmp; ++i) {
        int p = primes[i];
        if (tmp % p == 0) {
            int cnt = 0;
            while (tmp % p == 0) {
                ++cnt;
                if (cnt > 2) return false; /* exponent ≥3 */
                tmp /= p;
            }
            fac[pcnt] = p;
            exp[pcnt] = cnt;
            ++pcnt;
        }
    }
    if (tmp > 1) { /* remaining prime factor */
        if (tmp > (uint64_t)PRIMELIMIT) return false; /* too large to check */
        fac[pcnt] = tmp;
        exp[pcnt] = 1;
        ++pcnt;
    }

    /* pairwise checks */
    for (int i = 0; i < pcnt; ++i) {
        uint64_t pi = fac[i];
        int ei = exp[i];
        for (int j = i + 1; j < pcnt; ++j) {
            uint64_t pj = fac[j];
            int ej = exp[j];

            /* check pj against powers of pi */
            if ((pi - 1) % pj == 0) return false;
            if (ei == 2 && (pi + 1) % pj == 0) return false;

            /* check pi against powers of pj */
            if ((pj - 1) % pi == 0) return false;
            if (ej == 2 && (pj + 1) % pi == 0) return false;
        }
    }
    return true;
}

/* main function */
seq_t a(idx_t n)
{
    /* on first call ensure primes */
    init_primes();

    if (n == 1) return (seq_t)1; /* m=1 */

    unsigned long long count = 1;
    uint64_t candidate = 2;
    while (1) {
        if (is_abelian(candidate)) {
            ++count;
            if (count == n) return (seq_t)candidate;
        }
        ++candidate;
    }
    /* never reached */
    return (seq_t)0;
}
