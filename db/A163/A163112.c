/*
 * A163112  Prime numbers with gaps larger than 20 towards both neighboring
 *          primes.
 * https://oeis.org/A163112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p where p is a prime with gaps to both neighboring primes
 *            exceeding 20. The sequence is computed by generating primes in
 *            increasing order (using the utility function next_prime) and
 *            keeping a sliding window of three consecutive primes. Whenever
 *            the middle prime satisfies (prev - prevprev >...
 * Range:     u8 none, u16 n <= 33, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/next_prime.h"     /* uint64_t next_prime(uint64_t start); */

static seq_t *terms      = NULL;   /* array of sequence terms (prime values) */
static size_t terms_cnt  = 0;      /* how many terms have been stored     */
static size_t terms_cap  = 0;      /* current capacity of the array       */

static uint64_t prev = 0;   /* prime just before the current prime   */
static uint64_t cur  = 0;   /* current prime being examined          */
static bool initialized = false;      /* have we started generating primes?    */

/* Ensure the terms array can hold at least one more element. */
static void grow_terms(void)
{
    if (terms_cap == 0) {
        terms_cap = 16;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    } else {
        terms_cap <<= 1;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }
}

/* Generate terms until at least `need` terms are available. */
static void ensure_terms(idx_t need)
{
    size_t target = (size_t)need;

    if (!initialized) {
        /* Initialize the sliding window with the first primes. */
        prev = 2;
        cur  = 3;
        initialized = true;
    }

    while (terms_cnt < target) {
        /* Find the next prime strictly greater than the current one. */
        uint64_t next = next_prime(cur + 1);

        /* Check whether the current prime satisfies the gap conditions. */
        if (cur - prev > 20 && next - cur > 20) {
            if (terms_cnt == terms_cap)
                grow_terms();
            terms[terms_cnt++] = (seq_t)cur;
        }

        /* Slide the window: (prev, cur, next) → (cur, next, next_next). */
        prev = cur;
        cur  = next;
    }
}

/* Exported sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* sequence starts at index 1 */
    ensure_terms(n);
    return terms[(size_t)n - 1];
}
