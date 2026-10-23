/*
 * A125617  Sum of the squares of the quadratic nonresidues of prime(n).
 * https://oeis.org/A125617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=1}^{p-1, Legendre(k|p)=-1} k^2 * where p =
 *            prime(n). * * The function generates the n‑th prime by a local
 *            sieve (sufficient for * n ≤ 10 000). It then tests each residue
 *            a mod p for being a quadratic * non‑residue by evaluating
 *            a^((p‑1)/2) mod p (Euler's criterion). * No external ut...
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 419, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static void build_primes_upto(idx_t limit, uint32_t **primes_out, size_t *cnt_out)
{
    if (limit < 2) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }
    uint8_t *is_comp = (uint8_t*)calloc((size_t)limit + 1, 1);
    if (!is_comp) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!is_comp[i]) {
            for (idx_t j = i * i; j <= limit; j += i) is_comp[j] = 1;
        }
    }
    size_t count = 0;
    for (idx_t i = 2; i <= limit; ++i) if (!is_comp[i]) ++count;
    uint32_t *pr = (uint32_t*)malloc(count * sizeof(uint32_t));
    if (!pr) {
        free(is_comp);
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i) if (!is_comp[i]) pr[idx++] = (uint32_t)i;
    *primes_out = pr;
    *cnt_out = count;
    free(is_comp);
}

static uint64_t mod_pow_uint64(uint64_t a, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    uint64_t base = a % mod;
    while (exp) {
        if (exp & 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return res;
}

static uint32_t *prime_cache = NULL;
static size_t prime_cache_cnt = 0;

uint32_t get_cached_prime(idx_t n)
{
    const idx_t target = n;
    /* If the cache is uninitialized or too small, rebuild it. */
    if (prime_cache == NULL || prime_cache_cnt < (size_t)target) {
        /* Limit chosen to contain at least 10 000 primes (prime_10000 = 104 729). */
        const idx_t limit = 200000;
        uint32_t *tmp = NULL;
        size_t tmp_cnt = 0;
        build_primes_upto(limit, &tmp, &tmp_cnt);
        free(prime_cache);
        prime_cache = tmp;
        prime_cache_cnt = tmp_cnt;
    }
    /* n is 1‑based. */
    return prime_cache[(size_t)n - 1];
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* Outside OEIS offset, safe default */

    uint32_t p = get_cached_prime(n);
    seq_t sum = 0;

    if (p <= 2) return 0; /* p == 2 gives empty set of nonresidues */

    uint64_t prime = (uint64_t)p;
    uint64_t half = (prime - 1) / 2;

    for (uint64_t a_val = 1; a_val < prime; ++a_val) {
        uint64_t leg = mod_pow_uint64(a_val, half, prime);
        if (leg == prime - 1) { /* Legendre symbol = -1 -> nonresidue */
            seq_t sq = (seq_t)a_val * (seq_t)a_val;
            sum += sq;
        }
    }
    return sum;
}
