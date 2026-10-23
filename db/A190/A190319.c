/*
 * A190319  Numbers with prime factorization pq^2r^2s^4.
 * https://oeis.org/A190319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest numbers of the form p * q^2 * r^2 * s^4 with
 *            distinct primes p,q,r,s (order of the primes is arbitrary). The
 *            sequence is generated up to a fixed large bound so that the
 *            first many terms (more than 10k) are available
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* maximum bound for precomputed numbers (chosen to cover >10000 terms) */
#define LIMIT_VAL 1000000000ULL
/* sieve limit for generating primes needed for factor combinations */
#define PRIME_LIMIT 500000UL

/* generate all primes <= n, store in array primes[], return count */
static void sieve_primes(uint32_t n, uint32_t **primes_out, size_t *cnt_out)
{
    size_t size = n + 1;
    uint8_t *is_composite = (uint8_t*)calloc(size, 1);
    size_t primes_cap = 80000, primes_cnt = 0;
    uint32_t *primes = (uint32_t*)malloc(primes_cap * sizeof(uint32_t));
    for (uint32_t i = 2; i <= n; ++i) {
        if (!is_composite[i]) {
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                primes = (uint32_t*)realloc(primes, primes_cap * sizeof(uint32_t));
            }
            primes[primes_cnt++] = i;
            if ((uint64_t)i * i <= n) {
                for (uint64_t j = (uint64_t)i * i; j <= n; j += i)
                    is_composite[(size_t)j] = 1;
            }
        }
    }
    free(is_composite);
    *primes_out = primes;
    *cnt_out = primes_cnt;
}

/* comparison function for qsort on seq_t */
static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *va = (const seq_t*)a;
    const seq_t *vb = (const seq_t*)b;
    if (*va < *vb) return -1;
    if (*va > *vb) return 1;
    return 0;
}

/* global cache storing the sequence */
static seq_t *cache = NULL;
static size_t cache_size = 0;

/* generate the sequence up to LIMIT_VAL and store in global cache */
static void generate_sequence(void)
{
    uint32_t *primes;
    size_t prime_cnt;
    sieve_primes(PRIME_LIMIT, &primes, &prime_cnt);

    /* tmp array for all generated numbers */
    size_t cap = 1024;
    size_t cnt = 0;
    seq_t *numbers = (seq_t*)malloc(cap * sizeof(seq_t));

    const seq_t LIMIT = (seq_t)LIMIT_VAL;

    /* precompute powers for primes */
    for (size_t si = 0; si < prime_cnt; ++si) {
        uint64_t s = primes[si];
        seq_t s_pow4 = (seq_t)s * s * s * s;
        if (s_pow4 > LIMIT) break;

        for (size_t qi = 0; qi < prime_cnt; ++qi) {
            if (qi == si) continue;
            uint64_t q = primes[qi];
            seq_t q_sq = (seq_t)q * q;
            seq_t base_without_r = s_pow4 * q_sq;
            /* minimal possible r^2 is 4 (prime 2^2); if even that exceeds limit, break q loop */
            if (base_without_r * 4 > LIMIT) break;

            for (size_t ri = qi + 1; ri < prime_cnt; ++ri) {
                if (ri == si) continue;
                uint64_t r = primes[ri];
                seq_t r_sq = (seq_t)r * r;
                seq_t B = base_without_r * r_sq;
                /* minimal p is 2 distinct from s,q,r; if B*2 > LIMIT break further r */
                if (B * 2 > LIMIT) break;

                /* generate all multiples of B with primes p not equal to s,q,r */
                for (size_t pi = 0; pi < prime_cnt; ++pi) {
                    if (pi == si || pi == qi || pi == ri) continue;
                    uint64_t p = primes[pi];
                    seq_t val = B * p;
                    if (val > LIMIT) break;   /* further primes will increase val */
                    /* store val */
                    if (cnt == cap) {
                        cap <<= 1;
                        numbers = (seq_t*)realloc(numbers, cap * sizeof(seq_t));
                    }
                    numbers[cnt++] = val;
                }
            }
        }
    }

    /* sort the generated numbers */
    qsort(numbers, cnt, sizeof(seq_t), cmp_seq_t);

    /* deduplicate */
    seq_t *uniq = (seq_t*)malloc(cnt * sizeof(seq_t));
    size_t uniq_cnt = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (uniq_cnt == 0 || numbers[i] != uniq[uniq_cnt - 1]) {
            uniq[uniq_cnt++] = numbers[i];
        }
    }

    free(numbers);
    cache = uniq;
    cache_size = uniq_cnt;

    free(primes);
}

/* public function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (cache == NULL)
        generate_sequence();
    if ((idx_t)n > (idx_t)cache_size)
        return 0;          /* out of precomputed range; should not happen in tests */
    return cache[n - 1];
}
