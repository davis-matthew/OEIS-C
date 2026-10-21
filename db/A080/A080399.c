/*
 * A080399  Largest squarefree number dividing sum of squares of divisors of
 *          n.
 * https://oeis.org/A080399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007947(A001157(n)) (product of distinct primes of σ₂(n))
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 1857, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_PRIME_LIMIT 2000000   /* sqrt(σ₂(n)) ≈ 1.26·n, so ≈2·n covers n up to ~1e6 */

/* ---------- Prime sieve ------------------------------------------- */
static int  prime_cnt = 0;
static int  primes[MAX_PRIME_LIMIT / 10];   /* < 2e5 primes */
static bool primes_initialized = false;

static void init_primes(void)
{
    if (primes_initialized) return;
    bool *is_prime = (bool*)malloc((MAX_PRIME_LIMIT + 1) * sizeof(bool));
    if (!is_prime) exit(1);
    for (int i = 0; i <= MAX_PRIME_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= MAX_PRIME_LIMIT; ++i) if (is_prime[i]) {
        for (int j = i * i; j <= MAX_PRIME_LIMIT; j += i) is_prime[j] = false;
    }
    for (int i = 2; i <= MAX_PRIME_LIMIT; ++i) if (is_prime[i]) primes[prime_cnt++] = i;
    free(is_prime);
    primes_initialized = true;
}

/* ---------- Integer power for any seq_t --------------------------- */
static inline seq_t pow_seq(seq_t base, unsigned long long exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1ULL) result *= base;
        base *= base;
        exp >>= 1ULL;
    }
    return result;
}

/* ---------- Main function ------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* irrelevant, sequence starts at 1 */

    init_primes();

    /* ---- Factor n ---- */
    uint64_t nn = (uint64_t)n;
    int facp[20], fex[20];   /* enough for n < 2^64 */
    int fcnt = 0;

    for (int i = 0; i < prime_cnt; ++i) {
        int p = primes[i];
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        if (pp > nn) break;
        if (nn % p == 0) {
            int e = 0;
            while (nn % p == 0) { nn /= p; ++e; }
            facp[fcnt] = p;
            fex[fcnt] = e;
            ++fcnt;
        }
    }
    if (nn > 1) {                    /* remaining prime factor > MAX_PRIME_LIMIT */
        facp[fcnt] = (int)nn;
        fex[fcnt] = 1;
        ++fcnt;
    }

    /* ---- Compute σ₂(n) = Σ_{d|n} d² ---- */
    seq_t sigma2 = (seq_t)1;
    for (int i = 0; i < fcnt; ++i) {
        seq_t p = (seq_t)facp[i];
        unsigned long long e = (unsigned long long)fex[i];

        unsigned long long exp_num  = 2U * (e + 1U);   /* exponent for numerator */
        seq_t numerator   = pow_seq(p, exp_num) - (seq_t)1;
        seq_t denominator = pow_seq(p, 2U) - (seq_t)1;
        seq_t term = numerator / denominator;

        sigma2 *= term;
    }

    /* ---- Compute rad(σ₂(n)) ---- */
    seq_t rad   = (seq_t)1;
    seq_t tmp   = sigma2;
    for (int i = 0; i < prime_cnt; ++i) {
        seq_t pr = (seq_t)primes[i];
        if (pr * pr > tmp) break;
        if (tmp % pr == 0) {
            rad *= pr;           /* multiply each prime only once */
            while (tmp % pr == 0) tmp /= pr;
        }
    }
    if (tmp > (seq_t)1) rad *= tmp;   /* remaining prime factor > sqrt(sigma2) */

    return rad;
}
