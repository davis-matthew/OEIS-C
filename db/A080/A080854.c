/*
 * A080854  Numbers which can be expressed as the sum of two distinct primes
 *          in exactly five ways.
 * https://oeis.org/A080854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th positive integer that can be expressed as the
   sum of two distinct primes in exactly five ways
   (OEIS A080854).  The function computes the term on demand
   and caches all found terms up to a predefined limit. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

static const int SEARCH_LIMIT = 200000;      /* sufficient to cover all known terms */

static bool *prime_bits = NULL;              /* sieve array */
static uint32_t *prime_list = NULL;          /* list of primes ≤ SEARCH_LIMIT */
static size_t prime_count = 0;

static seq_t *term_list = NULL;              /* cached terms of the sequence */
static size_t term_count = 0;
static size_t term_capacity = 0;

/* ------------------------------------------------------------ */
/* Simple sieve of Eratosthenes that fills prime_list           */
/* ------------------------------------------------------------ */
static void init_primes(void)
{
    int limit = SEARCH_LIMIT;
    prime_bits = (bool *)malloc((limit + 1) * sizeof(bool));
    for (int i = 0; i <= limit; ++i) prime_bits[i] = true;
    prime_bits[0] = prime_bits[1] = false;

    int sq = (int)sqrt((double)limit) + 1;
    for (int i = 2; i <= sq; ++i) {
        if (prime_bits[i]) {
            for (int j = i * i; j <= limit; j += i) prime_bits[j] = false;
        }
    }

    /* count primes to allocate list */
    prime_count = 0;
    for (int i = 2; i <= limit; ++i) if (prime_bits[i]) ++prime_count;
    prime_list = (uint32_t *)malloc(prime_count * sizeof(uint32_t));

    size_t idx = 0;
    for (int i = 2; i <= limit; ++i) if (prime_bits[i]) prime_list[idx++] = (uint32_t)i;
}

/* ------------------------------------------------------------ */
/* Count unordered pairs of distinct primes summing to x      */
/* ------------------------------------------------------------ */
static int count_goldbach_parts(int x)
{
    int cnt = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        int p = (int)prime_list[i];
        if (p > x / 2) break;          /* p <= q */
        int q = x - p;
        if (q > p && prime_bits[q]) {
            ++cnt;
            if (cnt > 5) break;        /* early exit */
        }
    }
    return cnt;
}

/* ------------------------------------------------------------ */
/* Find all terms up to SEARCH_LIMIT (or until exhaustion)    */
/* ------------------------------------------------------------ */
static void init_terms(void)
{
    init_primes();

    term_capacity = 32;
    term_list = (seq_t *)malloc(term_capacity * sizeof(seq_t));
    term_count = 0;

    for (int n = 4; n <= SEARCH_LIMIT; ++n) {   /* smallest even > 2 */
        if (count_goldbach_parts(n) == 5) {
            if (term_count >= term_capacity) {
                term_capacity *= 2;
                term_list = (seq_t *)realloc(term_list, term_capacity * sizeof(seq_t));
            }
            term_list[term_count++] = (seq_t)n;
        }
    }
}

/* ------------------------------------------------------------ */
/* Public function required by the test harness               */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_terms();
        initialized = true;
    }

    if (n == 0) return 0;
    if (n <= term_count) return term_list[n - 1];
    /* if more terms are requested than exist, return 0 */
    return 0;
}
