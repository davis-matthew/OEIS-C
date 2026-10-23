/*
 * A141164  Numbers having exactly 1 divisor of the form 8*k + 7.
 * https://oeis.org/A141164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   This function generates numbers having exactly one divisor of
 *            the form 8*k + 7. The result a(n) is the nth such number (offset
 *            1). The algorithm factors each candidate integer and uses a
 *            convolution modulo 8 to count divisors congruent to 7
 * Range:     u8 n <= 91, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Utilities: prime generation up to 1,000,000 (sufficient for all
   numbers needed for n <= 10000) ------------------------------------ */
#define PRIME_SIEVE_LIMIT 1000000

static uint32_t prime_list[78498];   /* number of primes <= 1,000,000 */
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (prime_cnt > 0) return;               /* already initialized */

    uint8_t *is_composite = (uint8_t*)calloc(PRIME_SIEVE_LIMIT + 1, 1);
    if (!is_composite) exit(1);              /* allocation failed */

    for (uint32_t i = 2; (uint64_t)i * i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) {
            for (uint32_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                is_composite[j] = 1;
        }
    }

    for (uint32_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) prime_list[prime_cnt++] = i;
    }

    free(is_composite);
}

/* ------------------------------------------------------------------ */
/* Cache for the computed sequence ---------------------------------- */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;
static uint64_t next_to_test = 1;           /* next integer to examine */

static void append_cache(seq_t val)
{
    if (cache_size == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);               /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_size++] = val;
}

/* ------------------------------------------------------------------ */
/* Helper: test whether an integer has exactly one divisor ≡ 7 (mod 8) */
static bool has_exactly_one_divisor_7(uint64_t m)
{
    if (m < 7) return false;

    init_primes();

    /* Factor m (trial division using the pre‑computed primes). */
    uint32_t p_factors[64];
    uint32_t e_factors[64];
    size_t fcnt = 0;
    uint64_t temp = m;

    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= temp; ++i) {
        uint32_t p = prime_list[i];
        if (temp % p == 0) {
            uint32_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            p_factors[fcnt] = p;
            e_factors[fcnt] = e;
            ++fcnt;
        }
    }
    if (temp > 1) {                          /* residual prime factor */
        p_factors[fcnt] = (uint32_t)temp;
        e_factors[fcnt] = 1;
        ++fcnt;
    }

    /* Convolution of residue classes modulo 8. */
    seq_t counts[8] = {0};
    counts[1] = (seq_t)1;                    /* divisor 1 */

    for (size_t idx = 0; idx < fcnt; ++idx) {
        uint32_t p = p_factors[idx];
        uint32_t e = e_factors[idx];
        seq_t poly[8] = {0};

        if (p == 2) {
            /* 2^k mod 8: 1,2,4,0,0,... */
            poly[1] = 1;
            if (e >= 1) poly[2] = 1;
            if (e >= 2) poly[4] = 1;
            if (e >= 3) poly[0] = (seq_t)(e - 2);
        } else {
            uint32_t pm8 = p % 8;
            switch (pm8) {
                case 1:
                    poly[1] = (seq_t)(e + 1);
                    break;
                case 3: case 5: case 7:
                {
                    seq_t even_cnt = (seq_t)(e / 2 + 1);
                    seq_t odd_cnt  = (seq_t)((e + 1) / 2);
                    poly[1] = even_cnt;
                    poly[pm8] = odd_cnt;
                    break;
                }
                default:            /* impossible for odd primes */
                    break;
            }
        }

        /* Convolve counts with poly modulo 8. */
        seq_t new_counts[8] = {0};
        for (int r1 = 0; r1 < 8; ++r1) {
            if (counts[r1] == 0) continue;
            for (int r2 = 0; r2 < 8; ++r2) {
                if (poly[r2] == 0) continue;
                int r = (r1 * r2) & 7;              /* modulo 8 */
                new_counts[r] += counts[r1] * poly[r2];
            }
        }
        for (int r = 0; r < 8; ++r) counts[r] = new_counts[r];
    }

    return counts[7] == (seq_t)1;
}

/* ------------------------------------------------------------------ */
/* Sequence function -------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined, but safe */

    /* Generate terms until we have at least n of them. */
    while (cache_size < (size_t)n) {
        if (has_exactly_one_divisor_7(next_to_test))
            append_cache((seq_t)next_to_test);
        ++next_to_test;
    }
    return cache[n - 1];
}
