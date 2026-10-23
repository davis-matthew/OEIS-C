/*
 * A106964  Primes of the form 3x^2+2xy+7y^2, with x and y nonnegative.
 * https://oeis.org/A106964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes of the form 3x²+2xy+7y² with x,y≥0 (search / sieve)
 * Range:     u8 n <= 7, u16 n <= 708, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* the maximum value to generate – sufficient for the first 10 000 primes */
#define MAX_VAL 20000000ULL

/* small prime sieve limit – > sqrt(MAX_VAL) */
#define SMALL_PRIME_MAX 5000

/* helper structures (static globals) */
static bool small_is_prime[SMALL_PRIME_MAX + 1] = {0};
static uint32_t *small_primes = NULL;
static size_t small_prime_cnt = 0;

static uint8_t *seen = NULL;          /* values already produced */
static uint64_t *temp_vals = NULL;    /* temporary list of prime values */
static size_t temp_len = 0, temp_cap = 0;

static seq_t *prime_values = NULL;    /* final sorted list of primes */
static size_t prime_count = 0;

static void init_small_primes(void)
{
    size_t i, j;
    for (i = 0; i <= SMALL_PRIME_MAX; ++i) small_is_prime[i] = true;
    small_is_prime[0] = small_is_prime[1] = false;
    for (i = 2; i * i <= SMALL_PRIME_MAX; ++i)
        if (small_is_prime[i])
            for (j = i * i; j <= SMALL_PRIME_MAX; j += i)
                small_is_prime[j] = false;

    /* allocate array for the primes */
    small_prime_cnt = 0;
    for (i = 2; i <= SMALL_PRIME_MAX; ++i)
        if (small_is_prime[i]) ++small_prime_cnt;
    small_primes = (uint32_t *)malloc(small_prime_cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (i = 2; i <= SMALL_PRIME_MAX; ++i)
        if (small_is_prime[i]) small_primes[idx++] = (uint32_t)i;
}

static bool is_prime_small(uint64_t n)
{
    uint64_t limit = (uint64_t)floor(sqrt((double)n));
    for (size_t i = 0; i < small_prime_cnt; ++i) {
        uint64_t p = small_primes[i];
        if (p > limit) break;
        if (n % p == 0) return false;
    }
    return true;
}

static void push_temp(uint64_t v)
{
    if (temp_len == temp_cap) {
        size_t new_cap = temp_cap ? temp_cap * 2 : 1024;
        uint64_t *new_buf = (uint64_t *)realloc(temp_vals, new_cap * sizeof(uint64_t));
        if (!new_buf) exit(1);
        temp_vals = new_buf;
        temp_cap = new_cap;
    }
    temp_vals[temp_len++] = v;
}

static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va > vb) - (va < vb);
}

static void generate_sequence(void)
{
    /* preparation */
    seen = (uint8_t *)calloc(MAX_VAL + 1, sizeof(uint8_t));
    temp_vals = NULL;
    temp_len = 0;
    temp_cap = 0;

    /* outer loop over x */
    for (uint32_t x = 0;; ++x) {
        uint64_t xx = 3ULL * (uint64_t)x * (uint64_t)x;
        if (xx > MAX_VAL) break;
        for (uint32_t y = 0;; ++y) {
            uint64_t val = xx + 2ULL * (uint64_t)x * (uint64_t)y + 7ULL * (uint64_t)y * (uint64_t)y;
            if (val > MAX_VAL) break;
            if (val <= 1) continue;
            if (seen[val]) continue;
            if (is_prime_small(val)) {
                push_temp(val);
                seen[val] = 1;
            }
        }
    }

    /* sort the collected primes */
    qsort(temp_vals, temp_len, sizeof(uint64_t), cmp_uint64);

    /* store into seq_t array */
    prime_values = (seq_t *)malloc(temp_len * sizeof(seq_t));
    for (size_t i = 0; i < temp_len; ++i)
        prime_values[i] = (seq_t)temp_vals[i];
    prime_count = temp_len;

    /* free temporary arrays */
    free(temp_vals);
    free(seen);
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_small_primes();
        generate_sequence();
        initialized = true;
    }
    /* The sequence is 1‑based (offset 1) */
    if (n == 0 || n > prime_count) return 0;  /* out of bounds – return 0 */
    return prime_values[n - 1];
}
