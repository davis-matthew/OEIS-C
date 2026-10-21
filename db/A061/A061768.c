/*
 * A061768  k is the least number such that k! is divisible by (k+1)^n.
 * https://oeis.org/A061768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): minimal k such that k! is divisible by (k+1)^n */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- Types used by the build system ---------- */
extern seq_t a(idx_t n);

/* ---------- Constants ---------- */
#define INITIAL_LIMIT 2000000   /* enough for n up to about 10000 */

/* ---------- Global data ---------- */
static idx_t        current_k = 0;          /* last k processed           */
static size_t       max_n_computed = 0;     /* highest n solved already   */
static seq_t       *seq_values = NULL;      /* a[1..max_n_computed]       */
static size_t       seq_values_size = 0;    /* allocated size of seq_vals */

static int   *spf = NULL;                  /* smallest prime factor      */
static int   *prime_idx = NULL;            /* prime -> index in primes   */
static int   *primes = NULL;               /* list of primes             */
static size_t primes_cnt = 0;              /* number of primes           */
static uint32_t *prime_exp = NULL;         /* exponent of prime in k!    */
static size_t limit = 0;                   /* current sieve limit        */

/* ---------- Helper functions ---------- */
static void build_sieve_and_tables(void)
{
    /* allocate spf and construct sieve */
    spf = (int *)malloc((INITIAL_LIMIT + 1) * sizeof(int));
    for (size_t i = 0; i <= INITIAL_LIMIT; ++i) spf[i] = (int)i;
    for (size_t i = 2; i * i <= INITIAL_LIMIT; ++i) {
        if (spf[i] == (int)i) {
            for (size_t j = i * i; j <= INITIAL_LIMIT; j += i)
                if (spf[j] == (int)j) spf[j] = (int)i;
        }
    }

    /* collect primes */
    primes_cnt = 0;
    for (size_t i = 2; i <= INITIAL_LIMIT; ++i)
        if (spf[i] == (int)i) ++primes_cnt;

    primes = (int *)malloc(primes_cnt * sizeof(int));
    prime_idx = (int *)malloc((INITIAL_LIMIT + 1) * sizeof(int));
    for (size_t i = 0; i <= INITIAL_LIMIT; ++i) prime_idx[i] = -1;

    size_t idx = 0;
    for (size_t i = 2; i <= INITIAL_LIMIT; ++i)
        if (spf[i] == (int)i) {
            primes[idx] = (int)i;
            prime_idx[i] = (int)idx;
            ++idx;
        }

    prime_exp = (uint32_t *)calloc(primes_cnt, sizeof(uint32_t));
    limit = INITIAL_LIMIT;
}

static inline void factorize_and_add(idx_t v)
{
    /* add exponents of prime factors of v to prime_exp */
    while (v > 1) {
        int p = spf[v];
        uint32_t cnt = 0;
        while (v % p == 0) {
            v /= p;
            ++cnt;
        }
        int pi = prime_idx[p];
        prime_exp[pi] += cnt;
    }
}

static inline bool condition_ok(idx_t k, idx_t n)
{
    /* check whether k! is divisible by (k+1)^n */
    idx_t m = k + 1;
    while (m > 1) {
        int p = spf[m];
        uint32_t cnt = 0;
        while (m % p == 0) {
            m /= p;
            ++cnt;
        }
        int pi = prime_idx[p];
        if (((uint64_t)n * cnt) > prime_exp[pi]) return false;
    }
    return true;
}

static void ensure_seq_array(size_t w)
{
    if (seq_values_size >= w) return;
    size_t new_size = w;
    seq_t *new_arr = (seq_t *)realloc(seq_values, new_size * sizeof(seq_t));
    if (!new_arr) abort();
    seq_values = new_arr;
    seq_values_size = new_size;
}

/* ---------- Public function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* first call: initialise data structures */
    if (!limit) build_sieve_and_tables();

    /* allocate space for results up to n */
    ensure_seq_array((size_t)n + 1);

    /* compute missing terms up to n */
    for (size_t cur = max_n_computed + 1; cur <= (size_t)n; ++cur) {
        /* If this is the very first term, start k=1 */
        if (current_k == 0) {
            current_k = 1;
            factorize_and_add(1);
        }

        /* Advance k until the condition is satisfied */
        while (!condition_ok(current_k, (idx_t)cur)) {
            ++current_k;
            factorize_and_add(current_k);
        }
        seq_values[cur] = current_k;
    }
    max_n_computed = (size_t)n;
    return seq_values[n];
}
