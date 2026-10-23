/*
 * A039725  Even abundant numbers divided by 2.
 * https://oeis.org/A039725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A173490(n) / 2 – the nth even abundant number divided by
 *            two
 * Range:     u8 n <= 24, u16 n <= 4488, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code enumerates even numbers, tests for abundance using σ(n),
   and keeps the results in a growing cache so that calls in any order
   finish quickly.  It works for all idx_t/seq_t types supported by the
   build system. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Prime generation (once) ---------- */
#define PRIME_LIMIT 200000   /* covers factorisation of numbers up to ~10⁵ */

static uint32_t *prime_list = NULL;
static size_t prime_count = 0;

static void init_primes(void)
{
    if (prime_list != NULL)                /* already initialised */
        return;

    bool *is_comp = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!is_comp)                          /* allocation failed – abort */
        return;

    prime_list = (uint32_t *)malloc(PRIME_LIMIT * sizeof(uint32_t));
    if (!prime_list) {                     /* allocation failed – abort */
        free(is_comp);
        return;
    }

    prime_count = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[(size_t)i]) {
            prime_list[prime_count++] = (uint32_t)i;
            if (i * i <= PRIME_LIMIT)
                for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_comp[j] = true;
        }
    }
    free(is_comp);
}

/* ---------- Divisor‑sum σ(n) for uint64_t n ---------- */
static seq_t sigma_of(uint64_t n)
{
    seq_t result = 1;
    uint64_t temp = n;

    for (size_t idx = 0; idx < prime_count && (uint64_t)prime_list[idx] * prime_list[idx] <= temp; ++idx) {
        uint64_t p = prime_list[idx];
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {   /* count exponent */
                temp /= p;
                ++e;
            }
            /* σ(p^e) = 1 + p + p^2 + … + p^e */
            seq_t term = 1;
            seq_t power = 1;
            for (int i = 1; i <= e; ++i) {
                power *= (seq_t)p;
                term += power;
            }
            result *= term;
        }
    }

    /* Remaining prime factor > sqrt(n) */
    if (temp > 1) {
        seq_t pterm = 1 + (seq_t)temp;
        result *= pterm;
    }
    return result;
}

/* ---------- Check even abundance ---------- */
static bool is_even_abundant(uint64_t n)
{
    /* n is even */
    seq_t sigma = sigma_of(n);
    return sigma > ((seq_t)n * 2);
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)           /* undefined below offset */
        return 0;

    /* Initialise primes once */
    static bool primes_init_done = false;
    if (!primes_init_done) {
        init_primes();
        primes_init_done = true;
    }

    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t count = 0;
    static uint64_t last_even = 10;    /* next to test will be 12 */

    /* Allocate initial cache if needed */
    if (capacity == 0) {
        capacity = 16;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache)
            return 0;                /* allocation failed */
    }

    /* Generate terms until we reach desired index */
    while (count < (size_t)n) {
        last_even += 2;   /* consider next even number */
        if (is_even_abundant(last_even)) {
            /* Grow cache if necessary */
            if (count >= capacity) {
                size_t new_cap = capacity * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)              /* reallocation failed */
                    return 0;
                cache = tmp;
                capacity = new_cap;
            }
            cache[count++] = (seq_t)(last_even / 2);
        }
    }

    return cache[n - 1];
}
