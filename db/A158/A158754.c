/*
 * A158754  Twin prime numbers that can be written as the sum of two or more
 *          consecutive twin prime numbers.
 * https://oeis.org/A158754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: twin primes that can be written as the sum of two or more
 * consecutive twin primes.  Computed by sieving primes up to a fixed
 * limit, collecting the twin‑prime list, and then testing all
 * consecutive subsequences. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Resize a dynamic array – must cast the return of realloc in C++ */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Cache to avoid recomputing the sequence */
static seq_t *seq_cache = NULL;
static size_t seq_len   = 0;
static size_t seq_cap   = 0;

/* Build the sequence once and keep it in static storage */
static void compute_sequence(void)
{
    if (seq_len != 0) return;     /* already computed */

    /* 1. Sieve primes up to LIMIT -------------------------------------- */
    static const idx_t LIMIT = 10000000ULL;      /* 10 million, enough for n≈10 000 */
    uint8_t *is_prime = (uint8_t *)calloc((size_t)(LIMIT + 1), sizeof(uint8_t));
    if (!is_prime) abort();
    for (idx_t i = 0; i <= LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i) is_prime[j] = 0;
        }
    }

    /* 2. Build list of twin primes ------------------------------------- */
    seq_t *twin = NULL;
    size_t twin_cnt = 0, twin_cap = 0;
    for (idx_t p = 2; p <= LIMIT; ++p) {
        if (!is_prime[p]) continue;
        bool twin_flag = false;
        if (p >= 3 && is_prime[p - 2]) twin_flag = true;
        if (p + 2 <= LIMIT && is_prime[p + 2]) twin_flag = true;
        if (twin_flag) {
            grow_seq(&twin, &twin_cap, twin_cnt + 1);
            twin[twin_cnt++] = (seq_t)p;
        }
    }

    /* 3. Mark which numbers are twin primes --------------------------- */
    uint8_t *twin_present = (uint8_t *)calloc((size_t)(LIMIT + 1), sizeof(uint8_t));
    for (size_t i = 0; i < twin_cnt; ++i)
        twin_present[(idx_t)twin[i]] = 1;

    /* 4. Find sums of consecutive twin primes -------------------------- */
    uint8_t *rep = (uint8_t *)calloc((size_t)(LIMIT + 1), sizeof(uint8_t));
    for (size_t start = 0; start < twin_cnt; ++start) {
        seq_t sum = 0;
        for (size_t j = start; j < twin_cnt; ++j) {
            sum += twin[j];
            if (sum > LIMIT) break;
            if (j > start && twin_present[(idx_t)sum]) rep[(idx_t)sum] = 1;
        }
    }

    /* 5. Collect representable twin primes in ascending order ----------- */
    for (idx_t v = 0; v <= LIMIT; ++v) {
        if (rep[v]) {
            grow_seq(&seq_cache, &seq_cap, seq_len + 1);
            seq_cache[seq_len++] = (seq_t)v;
        }
    }

    /* 6. Clean up temporary arrays ------------------------------------- */
    free(is_prime);
    free(twin);
    free(twin_present);
    free(rep);
}

/* Public entry point required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined for offset < 1 */
    compute_sequence();
    if (n - 1 < seq_len) return seq_cache[n - 1];
    return 0;                     /* n beyond computed range */
}
