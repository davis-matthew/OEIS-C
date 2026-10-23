/*
 * A192189  Polynomial-like numbers: numbers whose sequence of
 *          number-derivatives (A003415) is monotonically decreasing.
 * https://oeis.org/A192189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer whose arithmetic‑derivative
 *            sequence is strictly decreasing
 * Range:     u8 n <= 78, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Helper data structures for prime generation ------------------------------------ */
#define PRIME_LIMIT 1000000          /* sieve up to one million */
#define PRIME_CAP   200000           /* maximum number of primes we keep */

static uint64_t primes[PRIME_CAP];
static size_t   prime_cnt = 0;
static bool     primes_initialized = false;

static void init_primes(void)
{
    if (primes_initialized) return;
    bool *is_prime = (bool*)malloc(PRIME_LIMIT + 1);
    if (!is_prime) exit(1);

    memset(is_prime, 1, PRIME_LIMIT + 1);
    is_prime[0] = is_prime[1] = 0;

    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    for (size_t i = 2; i <= PRIME_LIMIT && prime_cnt < PRIME_CAP; ++i) {
        if (is_prime[i]) primes[prime_cnt++] = i;
    }
    free(is_prime);
    primes_initialized = true;
}

/* Arithmetic derivative of n:  n' = sum_{p^e||n} (n/p) * e  --------------------- */
static seq_t derivative(seq_t n)
{
    if (n <= 1) return 0;
    seq_t res = 0;
    seq_t temp = n;

    for (size_t i = 0; i < prime_cnt && (seq_t)primes[i] * (seq_t)primes[i] <= temp; ++i) {
        seq_t p = primes[i];
        if (temp % p == 0) {
            unsigned long long e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            res += (n / p) * (seq_t)e;
        }
    }
    if (temp > 1) {          /* remaining prime factor > sqrt(n) */
        seq_t p = temp;
        res += (n / p);      /* e = 1 */
    }
    return res;
}

/* Test whether a given integer belongs to the sequence -------------------------- */
static bool is_poly_like(seq_t n)
{
    if (n <= 1) return true;
    seq_t m = n;
    while (m > 1) {
        seq_t next = derivative(m);
        if (next >= m) return false;
        m = next;
    }
    return true;
}

/* Storage for previously computed sequence values ------------------------------- */
static seq_t *seq_vals   = NULL;
static idx_t   seq_len   = 0;
static idx_t   seq_cap   = 0;
static seq_t   next_candidate = 1;

static void ensure_seq_capacity(idx_t needed)
{
    if (needed <= seq_cap) return;
    idx_t new_cap = seq_cap == 0 ? 16 : seq_cap * 2;
    if (new_cap < needed) new_cap = needed * 2;
    seq_t *tmp = (seq_t*)realloc(seq_vals, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    seq_vals = tmp;
    seq_cap  = new_cap;
}

/* Main function required by the problem --------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined; just return 0 */
    init_primes();

    while (seq_len < n) {
        if (is_poly_like(next_candidate)) {
            ensure_seq_capacity(seq_len + 1);
            seq_vals[seq_len++] = next_candidate;
        }
        ++next_candidate;
    }
    return seq_vals[n - 1];
}
