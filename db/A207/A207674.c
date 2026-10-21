/*
 * A207674  Numbers such that all divisors occur in their Collatz
 *          trajectories.
 * https://oeis.org/A207674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer all of whose divisors belong to
 *            its Collatz trajectory
 * Range:     u8 n <= 57, u16 n <= 1913, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

/* ----------  Simple prime sieve up to 1,000,000  ---------- */
#define PRIME_LIMIT 1000000
static uint32_t prime_list[78498];
static size_t prime_count = 0;
static bool primes_inited = false;

static void init_primes(void)
{
    if (primes_inited) return;
    primes_inited = true;
    bool *is_composite = (bool*)calloc(PRIME_LIMIT + 1, sizeof(bool));
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            prime_list[prime_count++] = (uint32_t)i;
            if (i * i <= PRIME_LIMIT) {
                for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }
    free(is_composite);
}

/* ----------  Factorisation of a seq_t using trial division  ---------- */
static void factor_seq(seq_t n, uint64_t *primes, int *exps, size_t *cnt)
{
    *cnt = 0;
    seq_t m = n;
    if (m == (seq_t)0) return;

    init_primes();

    for (size_t i = 0; i < prime_count; ++i) {
        seq_t p = (seq_t)prime_list[i];
        seq_t p2 = p * p;
        if (p2 > m) break;
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m = m / p;
                ++e;
            }
            primes[*cnt] = (uint64_t)prime_list[i];
            exps[*cnt] = e;
            ++(*cnt);
        }
    }
    /* Remaining factor, if any */
    if (m > (seq_t)1) {
        /* m may be prime or a product of two large primes.
           Treat as a single prime factor; for the purpose of generating divisors
           this is sufficient because we only need to know all divisors, not
           whether they are prime. */
        primes[*cnt] = (uint64_t)m;
        exps[*cnt] = 1;
        ++(*cnt);
    }
}

/* ----------  Generate all divisors from prime list  ---------- */
static size_t generate_divisors(const uint64_t *pr,
                                const int *exp,
                                size_t pcnt,
                                seq_t *divs)
{
    size_t dcnt = 0;
    divs[dcnt++] = (seq_t)1;
    for (size_t i = 0; i < pcnt; ++i) {
        size_t cur_cnt = dcnt;
        seq_t mult = (seq_t)1;
        for (int e = 1; e <= exp[i]; ++e) {
            mult = mult * (seq_t)pr[i];
            for (size_t j = 0; j < cur_cnt; ++j)
                divs[dcnt++] = divs[j] * mult;
        }
    }
    return dcnt;
}

/* ----------  Check if all divisors of m occur in its Collatz trajectory ---------- */
static bool has_all_divisors(seq_t m)
{
    /* Factorise m */
    uint64_t primes[64];
    int exps[64];
    size_t pcnt;
    factor_seq(m, primes, exps, &pcnt);

    seq_t divs[4096];
    size_t divcnt = generate_divisors(primes, exps, pcnt, divs);
    if (divcnt > 4096) return false; /* safety guard */

    bool found[4096] = {false};
    size_t remaining = divcnt;

    seq_t cur = m;
    while (true) {
        if (m % cur == 0) {
            /* cur is a divisor of m */
            for (size_t i = 0; i < divcnt; ++i) {
                if (divs[i] == cur && !found[i]) {
                    found[i] = true;
                    if (--remaining == 0) return true;
                    break;
                }
            }
        }
        if (cur == (seq_t)1) break;

        /* next Collatz step */
        if ((cur & (seq_t)1) == 0) /* even */
            cur = cur >> 1;
        else
            cur = cur * (seq_t)3 + (seq_t)1;
    }
    return remaining == 0;
}

/* ----------  Cache for sequence values ---------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t newcap = cache_cap ? cache_cap : 8;
    while (newcap < need) newcap <<= 1;
    cache = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
    cache_cap = newcap;
}

/* ----------  Main function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n=0 */

    static seq_t next_candidate = (seq_t)1;
    while (cache_len < n) {
        if (has_all_divisors(next_candidate)) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = next_candidate;
        }
        next_candidate = next_candidate + (seq_t)1;
    }
    return cache[n - 1];
}
