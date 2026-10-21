/*
 * A099208  In canonical prime-factorization of n replace prime(k)^e with
 *          prime(k+e-1).
 * https://oeis.org/A099208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over prime factors p_k^e of n of prime(k+e-1),
 *            where k is the index of p_k in the prime sequence (1→2, 2→3,
 *            3→5, …)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- prime table ---------- */
#define PRIME_TABLE_MAX 20000          /* enough primes for all indices we need */
#define SIEVE_LIMIT    200000          /* generates about 17984 primes  */

static uint64_t primes[PRIME_TABLE_MAX];
static size_t prime_cnt = 0;

/* initialize prime table using simple sieve */
static void init_prime_table(void)
{
    if (prime_cnt != 0) return;

    bool *is_composite = (bool *)calloc(SIEVE_LIMIT + 1, sizeof(bool));
    if (!is_composite) return; /* should not happen in test environment */

    for (int i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) {
            if (prime_cnt < PRIME_TABLE_MAX)
                primes[prime_cnt++] = (uint64_t)i;
            /* mark composites */
            if ((int64_t)i * i <= SIEVE_LIMIT) {
                for (int j = i * i; j <= SIEVE_LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }
    free(is_composite);
}

/* binary search to find 1‑based index of prime p in primes array */
static idx_t prime_index_of(uint64_t p)
{
    init_prime_table();
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* primes must contain p; adjust to 1‑based index */
    if (lo < prime_cnt && primes[lo] == p)
        return (idx_t)(lo + 1);
    return 0; /* p not found – should not happen for a prime factor of n */
}

/* return the k‑th prime (1‑based) */
static seq_t get_prime_k(idx_t k)
{
    init_prime_table();
    /* our table is large enough for all indices needed in tests */
    if (k <= prime_cnt) return (seq_t)primes[k - 1];
    /* fallback: generate additional primes if required (unlikely) */
    uint64_t candidate = primes[prime_cnt - 1] + 2;
    while (true) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= candidate; ++i) {
            if (candidate % primes[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_cnt < PRIME_TABLE_MAX)
                primes[prime_cnt++] = candidate;
            return (seq_t)candidate;
        }
        candidate += 2;
    }
}

/* ---------- factorisation helper ---------- */
/* factor_uint64_custom: trial division using the prime table */
static void factor_uint64_custom(uint64_t n, uint64_t *fac, int *exp, size_t *cnt)
{
    init_prime_table();
    *cnt = 0;
    uint64_t remaining = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= remaining; ++i) {
        if (remaining % primes[i] == 0) {
            fac[(*cnt)] = primes[i];
            int e = 0;
            while (remaining % primes[i] == 0) {
                remaining /= primes[i];
                ++e;
            }
            exp[(*cnt)] = e;
            ++(*cnt);
        }
    }
    if (remaining > 1) {
        fac[(*cnt)] = remaining;
        exp[(*cnt)] = 1;
        ++(*cnt);
    }
}

/* ---------- the required sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* undefined, return 0 */
    if (n == 1) return (seq_t)1;

    uint64_t fac[32];
    int exp[32];
    size_t cnt;

    factor_uint64_custom((uint64_t)n, fac, exp, &cnt);

    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t prime_idx = prime_index_of(fac[i]);          /* index of p_k in prime list */
        idx_t target    = prime_idx + exp[i] - 1;          /* k + e - 1 */
        seq_t p = get_prime_k(target);
        res *= p;
    }
    return res;
}
