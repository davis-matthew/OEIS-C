/*
 * A063381  Number of orbits of the group of units of Z/(n) acting naturally
 *          on the 4-subsets of Z/(n).
 * https://oeis.org/A063381
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   C2, C3, C4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A063381
 *  Formula: a(n) = (1/φ(n)) * Σ_{u∈U(n)} f(u),  where
 *  f(u) counts the 4‑subsets of Z/n fixed by multiplying
 *  by u.  For a permutation with cycle lengths l_i,
 *  f(u) = a4 + a3·a1 + C(a2,2) + a2·C(a1,2) + C(a1,4),
 *  where aL = number of cycles of length L.  For each u we
 *  only need cycle lengths ≤4, which are determined by the
 *  multiplicative order of u modulo divisors of n. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/C2.h"
#include "../utils/C3.h"
#include "../utils/C4.h"

/* ---------- basic utilities ---------- */
static inline uint64_t mygcd(uint64_t a, uint64_t b) {
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

/* fast modular exponentiation (uint64_t) */
static inline uint64_t powmod_uint64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1) res = (unsigned __int128)res * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1;
    }
    return res;
}

/* ---------- sieve of primes up to 10000 ---------- */
#define PRIME_MAX 10000
static int prime_list[PRIME_MAX / 2];
static int prime_cnt = 0;

static void init_primes(void) {
    if (prime_cnt) return;
    bool is_composite[PRIME_MAX + 1] = {false};
    for (int i = 2; i <= PRIME_MAX; ++i) {
        if (!is_composite[i]) {
            prime_list[prime_cnt++] = i;
            if (1LL * i * i <= PRIME_MAX)
                for (int j = i * i; j <= PRIME_MAX; j += i)
                    is_composite[j] = true;
        }
    }
}

/* factorization into distinct primes (max 10 distinct primes for n≤10000) */
static int factor_distinct(uint64_t n, int res[10]) {
    int cnt = 0;
    for (int i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= n; ++i) {
        int p = prime_list[i];
        if (n % p == 0) {
            res[cnt++] = p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) res[cnt++] = (int)n;
    return cnt;
}

/* Euler totient of n (n≤10000) */
static uint64_t phi_uint64(uint64_t n) {
    uint64_t res = n;
    int primes[10];
    int cnt = factor_distinct(n, primes);
    for (int i = 0; i < cnt; ++i) {
        int p = primes[i];
        res = res / p * (p - 1);
    }
    return res;
}

/* ---------- main function ---------- */
seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)0;          /* value not required for n<4 */

    init_primes();

    /* list of divisors of n (excluding 1 for later use) */
    uint64_t divs[128];
    int div_cnt = 0;
    for (uint64_t d = 1; d * d <= (uint64_t)n; ++d) {
        if ((uint64_t)n % d == 0) {
            divs[div_cnt++] = d;
            if (d * d != (uint64_t)n) divs[div_cnt++] = (uint64_t)n / d;
        }
    }

    /* precompute φ(m) and its prime factors for each divisor m > 1 */
    struct DivInfo { uint64_t m; uint64_t phi; int primes[10]; int pcnt; } divinfo[128];
    int di_cnt = 0;
    for (int i = 0; i < div_cnt; ++i) {
        uint64_t m = divs[i];
        if (m <= 1) continue;
        divinfo[di_cnt].m = m;
        divinfo[di_cnt].phi = phi_uint64(m);
        divinfo[di_cnt].pcnt = factor_distinct(divinfo[di_cnt].phi, divinfo[di_cnt].primes);
        ++di_cnt;
    }

    /* totient of n for division at the end */
    uint64_t phi_n = phi_uint64((uint64_t)n);

    /* accumulate sum of fixed 4‑subsets over all units */
    unsigned __int128 sum_fixed = 0;

    /* iterate over all units u (1 ≤ u < n with gcd(u,n)=1) */
    for (uint64_t u = 1; u < (uint64_t)n; ++u) {
        if (mygcd(u, (uint64_t)n) != 1) continue ;

        uint64_t a_len[5] = {0,0,0,0,0};  /* a_len[1..4] */
        /* zero always a cycle of length 1 */
        a_len[1] = 1;

        /* process each divisor m > 1 */
        for (int di = 0; di < di_cnt; ++di) {
            uint64_t m = divinfo[di].m;
            uint64_t phi_m = divinfo[di].phi;
            /* multiplicative order of u modulo m */
            uint64_t order = phi_m;
            uint64_t base = u % m;
            for (int pi = 0; pi < divinfo[di].pcnt; ++pi) {
                int p = divinfo[di].primes[pi];
                while (order % p == 0) {
                    uint64_t exp = order / p;
                    if (powmod_uint64(base, exp, m) == 1 % m) {
                        order = exp;
                    } else break;
                }
            }
            /* order now equals minimal positive t with u^t ≡ 1 (mod m) */
            uint64_t L = order;
            if (L <= 4) {
                /* number of cycles of this length contributed by elements with gcd(a,n)=n/m */
                uint64_t cycles = phi_m / L;
                a_len[L] += cycles;
            }
        }

        /* now compute fixed 4-subsets for this unit */
        uint64_t a1 = a_len[1];
        uint64_t a2 = a_len[2];
        uint64_t a3 = a_len[3];
        uint64_t a4 = a_len[4];

        uint64_t fixed = 0;
        fixed += a4;                            /* one cycle of length 4 */
        fixed += a3 * a1;                       /* 3+1 */
        fixed += (a2 * (a2 - 1)) / 2;           /* 2+2 */
        fixed += a2 * (a1 * (a1 - 1)) / 2;      /* 2+1+1 */
        fixed += (a1 * (a1 - 1) * (a1 - 2) * (a1 - 3)) / 24; /* 1+1+1+1 */

        sum_fixed += fixed;
    }

    /* average over φ(n) */
    seq_t result = (seq_t)(sum_fixed / phi_n);
    return result;
}
