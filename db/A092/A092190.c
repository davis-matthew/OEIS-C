/*
 * A092190  Semiprimes that are the sum of the first n semiprimes for some n.
 * https://oeis.org/A092190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 51, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = the n-th semiprime that equals the sum of the first k semiprimes for some k.
 *  The function generates semiprimes in increasing order, keeps a running sum,
 *  and records the sum whenever it itself is a semiprime.
 *  Subsequent calls reuse the cached list.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static unsigned *primes = NULL;
static size_t primes_len = 0;
static int primes_initialized = 0;

static seq_t *term_array = NULL;
static idx_t term_count = 0;
static size_t term_capacity = 0;

static uint64_t current_semiprime = 4;     /* next candidate for semiprime */
static uint64_t cumulative_sum  = 0;       /* running sum of found semiprimes */

/* simple prime sieve up to a fixed limit (chosen to cover required sums) */
static void init_primes(void)
{
    if (primes_initialized) return;
    const size_t LIMIT = 2000000u;      /* 2 million is safe for the problem limits */
    unsigned char *sieve = (unsigned char *)malloc(LIMIT + 1);
    if (!sieve) abort();
    memset(sieve, 0, LIMIT + 1);

    for (size_t i = 2; i * i <= LIMIT; ++i)
        if (!sieve[i])
            for (size_t j = i * i; j <= LIMIT; j += i)
                sieve[j] = 1;

    /* first count primes to allocate array */
    size_t count = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (!sieve[i]) ++count;

    primes = (unsigned *)malloc(count * sizeof(unsigned));
    if (!primes) abort();
    primes_len = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (!sieve[i]) primes[primes_len++] = (unsigned)i;

    free(sieve);
    primes_initialized = 1;
}

/* test if x is a semiprime (product of exactly two primes counting multiplicity) */
static int is_semiprime(uint64_t x)
{
    int cnt = 0;
    uint64_t t = x;
    for (size_t i = 0; i < primes_len && (uint64_t)primes[i] * (uint64_t)primes[i] <= t; ++i) {
        unsigned int p = primes[i];
        if (t % p == 0) {
            ++cnt;
            t /= p;
            while (t % p == 0) {
                ++cnt;
                t /= p;
                if (cnt > 2) return 0;
            }
            if (cnt > 2) return 0;
        }
    }
    if (t > 1) ++cnt;           /* remaining prime factor */
    return cnt == 2;
}

/* ensure term_array has room for at least n elements */
static void ensure_terms_capacity(idx_t n)
{
    if (term_capacity < (size_t)n) {
        size_t new_cap = term_capacity ? term_capacity * 2 : 16;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(term_array, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        term_array = tmp;
        term_capacity = new_cap;
    }
}

/* main function requested by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n<1, return 0 */

    init_primes();                      /* primes must be ready */

    /* grow the list until we have at least n terms */
    while (term_count < (size_t)n) {
        /* find next semiprime */
        while (!is_semiprime(current_semiprime)) ++current_semiprime;
        cumulative_sum += current_semiprime;

        if (is_semiprime(cumulative_sum)) {
            ensure_terms_capacity(term_count + 1);
            term_array[term_count++] = (seq_t)cumulative_sum;
        }
        ++current_semiprime;
    }

    return term_array[n - 1];
}
