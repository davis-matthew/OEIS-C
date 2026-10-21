/*
 * A216091  Numbers n such that k == k^(q-1) mod q for k = 1, 2, ..., q-1,
 *          where q = n^2+1.
 * https://oeis.org/A216091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer m such that for * q = m^2 + 1, * every k
 *            in {1,…,q-1} satisfies k^(q-1) ≡ k (mod q). * The property is
 *            equivalent to m being odd and the odd part of q * (= (m^2+1)/2)
 *            being square‑free with every prime divisor p * satisfying (p-1)
 *            | (m^2-1). * The function returns the nth...
 * Range:     u8 n <= 44, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool primes_generated = false;
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;

/* Simple sieve: generate primes up to LIMIT (inclusive). */
static void generate_primes(uint32_t LIMIT)
{
    /* Allocate boolean array for sieve */
    bool *is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime) return; /* should not happen */
    for (uint32_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (uint32_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint32_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
        }
    }
    /* Count primes first to allocate array */
    prime_count = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) ++prime_count;
    prime_list = (uint32_t *)malloc(prime_count * sizeof(uint32_t));
    if (!prime_list) { free(is_prime); return; }
    size_t pos = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) prime_list[pos++] = i;
    free(is_prime);
    primes_generated = true;
}

/* Test whether an integer cand (odd) satisfies the OEIS condition. */
static bool valid_candidate(uint64_t cand)
{
    if ((cand & 1ULL) == 0) return false;          /* need odd candidate */
    uint64_t q = cand * cand + 1ULL;
    if (q & 1ULL) return false;                   /* q must be even for odd cand */

    uint64_t m = q >> 1;                           /* odd part of q */
    uint64_t tmp = m;
    uint64_t n_sq_minus1 = cand * cand - 1ULL;

    bool squarefree = true;
    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * prime_list[i] <= tmp; ++i) {
        uint64_t p = prime_list[i];
        if (tmp % p == 0) {
            uint64_t count = 0;
            do { tmp /= p; ++count; } while (tmp % p == 0);
            if (count > 1) { squarefree = false; break; }
            if ((n_sq_minus1 % (p - 1)) != 0) { squarefree = false; break; }
        }
    }
    if (!squarefree) return false;
    if (tmp > 1) {                /* remaining prime factor */
        uint64_t p = tmp;
        if ((n_sq_minus1 % (p - 1)) != 0) return false;
    }
    return true;
}

/* The sequence table, growing as needed. */
static seq_t *seq_table = NULL;
static size_t seq_size  = 0;      /* number of terms stored */
static size_t seq_cap   = 0;      /* allocated capacity */
static uint64_t next_candidate = 1;   /* next integer to test */

/* Ensure the prime list is generated. */
static void maybe_generate_primes(void)
{
    if (!primes_generated) {
        /* 200000 is comfortably above sqrt of (max_n^2)/2 for n up to ~100000 */
        generate_primes(200000);
    }
}

/* Expand the sequence table if necessary. */
static void ensure_capacity(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 128;
    while (new_cap < needed) new_cap *= 2;
    seq_table = (seq_t *)realloc(seq_table, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for 0, just return 0 */

    maybe_generate_primes();

    /* Generate terms until we have at least n terms. */
    while (seq_size < n) {
        /* Find next valid candidate. */
        uint64_t cand = next_candidate;
        while (!valid_candidate(cand)) ++cand;
        /* Store candidate. */
        ensure_capacity(seq_size + 1);
        seq_table[seq_size++] = (seq_t)cand;
        next_candidate = cand + 1;
    }

    return seq_table[n - 1];
}
