/*
 * A053571  Sum of totient functions over arguments running through numbers
 *          unrelated to n.
 * https://oeis.org/A053571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{1 ≤ m ≤ n, gcd(m,n)>1, m∤n} φ(m)
 * Range:     u8 n <= 41, u16 n <= 629, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ----------  helper data for sieve and factorization  ---------- */
static uint64_t *spf = NULL;       /* smallest prime factor for each integer */
static size_t spf_size = 0;        /* allocated size of spf array  */
static uint64_t *primes = NULL;    /* prime list for sieve */
static size_t prime_cap = 0;       /* allocated size of primes array */
static size_t prime_cnt = 0;       /* number of primes found */

/* ----------  initialise sieve up to N  ---------- */
static void init_sieve(idx_t N)
{
    if (spf_size > N) return;  /* already enough */

    /* allocate arrays */
    spf = (uint64_t*)realloc(spf, (N+1) * sizeof(uint64_t));
    if (prime_cap < N+1) {
        prime_cap = N+1;
        primes = (uint64_t*)realloc(primes, prime_cap * sizeof(uint64_t));
    }

    /* linear sieve */
    prime_cnt = 0;
    for (idx_t i = 0; i <= N; ++i) spf[i] = 0;
    for (idx_t i = 2; i <= N; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes[prime_cnt++] = i;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint64_t p = primes[j];
            idx_t ip = i * p;
            if (ip > N) break;
            spf[ip] = p;
            if (i % p == 0) break;
        }
    }
    spf_size = N + 1;
}

/* ----------  compute φ(x) using the pre‑computed spf  ---------- */
static inline seq_t phi_of(idx_t x)
{
    seq_t res = (seq_t)x;
    idx_t last_p = 0;
    while (x > 1) {
        uint64_t p = spf[x];
        if (p != last_p) {
            res -= res / (seq_t)p;
            last_p = p;
        }
        x /= p;
    }
    return res;
}

/* ----------  recursive enumeration of all divisors of n  ---------- */
static inline void dfs_divs(const uint64_t *p, const int *e,
                            size_t idx, size_t total,
                            idx_t cur, seq_t *sum)
{
    if (idx == total) {
        if (cur > 1) *sum += phi_of(cur);
        return;
    }
    idx_t prod = 1;
    for (int k = 0; k <= e[idx]; ++k) {
        dfs_divs(p, e, idx+1, total, cur * prod, sum);
        prod *= p[idx];
    }
}

/* ----------  main sequence function  ---------- */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* a(1)=0, a(2)=0 handled here */

    /* prepare sieve data */
    init_sieve(n);

    /* factorise n to distinct primes */
    const size_t MAX_FAC = 64;
    uint64_t facs[MAX_FAC];
    int    exps[MAX_FAC];
    size_t pfcnt = 0;
    idx_t tmp = n;
    while (tmp > 1) {
        uint64_t p = spf[tmp];
        int cnt = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++cnt;
        }
        facs[pfcnt] = p;
        exps[pfcnt] = cnt;
        ++pfcnt;
    }

    /* inclusion–exclusion over prime subsets for multiples of at least one prime */
    seq_t sum_multiples = (seq_t)0;
    uint64_t subsets = (uint64_t)1 << pfcnt;
    for (uint64_t mask = 1; mask < subsets; ++mask) {
        idx_t d = 1;
        size_t bits = 0;
        for (size_t i = 0; i < pfcnt; ++i) {
            if (mask & ((uint64_t)1 << i)) {
                d *= facs[i];
                ++bits;
            }
        }
        seq_t smul = (seq_t)0;
        for (idx_t mval = d; mval <= n; mval += d) {
            smul += phi_of(mval);
        }
        if (bits & 1) sum_multiples += smul;
        else           sum_multiples -= smul;
    }

    /* sum φ(d) over all proper divisors d>1 of n */
    seq_t sum_divs = (seq_t)0;
    dfs_divs(facs, exps, 0, pfcnt, 1, &sum_divs);

    seq_t result = sum_multiples;
    result -= sum_divs;                /* subtract divisors' totients */
    return result;
}
