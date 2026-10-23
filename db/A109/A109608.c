/*
 * A109608  Numbers n such that the number of digits required to write the
 *          prime factors of n equals the number of digits of n.
 * https://oeis.org/A109608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is obtained by searching numbers n such that the number of
 *            decimal digits of n equals the sum of the decimal digits of its
 *            prime factors (each occurrence counted separately)
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_digits, digit_len, push_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Utility function prototypes (provided by the build environment). */
#include "../utils/count_digits.h"   /* int count_digits(idx_t x);          */
#include "../utils/digit_len.h"      /* idx_t digit_len(seq_t x);            */
#include "../utils/push_seq.h"       /* void push_seq(seq_t **arr, idx_t *cap, idx_t *size, seq_t val); */

#define MAX_PRIME_LIMIT 200000          /* Upper bound for sieve; large enough for the needed terms. */
#define MAX_PRIMES       18000          /* Roughly the number of primes ≤ 200000. */

static uint32_t primes[MAX_PRIMES];
static size_t   prime_count = 0;

/* Initialise the prime table once. */
static void init_primes(void)
{
    static bool done = false;
    if (done) return;
    static bool sieve[MAX_PRIME_LIMIT + 1];
    /* Initially assume all are prime. */
    for (size_t i = 0; i <= MAX_PRIME_LIMIT; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (size_t i = 2; i * i <= MAX_PRIME_LIMIT; ++i)
        if (sieve[i])
            for (size_t j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                sieve[j] = false;
    for (size_t i = 2; i <= MAX_PRIME_LIMIT; ++i)
        if (sieve[i])
            primes[prime_count++] = (uint32_t)i;
    done = true;
}

/* Test if a given integer n satisfies the sequence property. */
static bool is_term(idx_t n)
{
    int ndig = count_digits(n);
    idx_t m = n;
    seq_t sum = 0;
    for (size_t i = 0; i < prime_count && (uint64_t)primes[i] * primes[i] <= m; ++i) {
        uint32_t p = primes[i];
        if (m % p == 0) {
            while (m % p == 0) {
                m /= p;
                sum += digit_len((seq_t)p);
            }
        }
    }
    if (m > 1) {
        sum += digit_len((seq_t)m);   /* m itself is a prime > sqrt(n) */
    }
    return ((idx_t)sum) == ndig;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    /* Static storage for the sequence values. */
    static seq_t *terms = NULL;
    static idx_t term_cap = 0;
    static idx_t term_size = 0;
    static idx_t next_candidate = 2;   /* The next integer to test. */

    /* Initialise prime table once. */
    init_primes();

    /* Generate terms until we have at least n of them. */
    while (term_size < n) {
        if (is_term(next_candidate)) {
            push_seq(&terms, &term_cap, &term_size, (seq_t)next_candidate);
        }
        ++next_candidate;
    }
    return terms[n - 1];
}
