/*
 * A165934  Primes from twin primes by taking the factorial of each digit and
 *          adding them up.
 * https://oeis.org/A165934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = sum of factorial of each decimal digit of the lower and
 *            upper prime of the n-th twin prime pair; only terms where this
 *            sum is prime are listed, in the order of the twin primes
 * Range:     u8 none, u16 n <= 1, u32 n <= 1, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   build_primes, factorial, grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>      /* size_t */
#include <stdint.h>      /* uint64_t */
#include <stdlib.h>      /* malloc, free */
#include <stdbool.h>     /* bool */

#include "../utils/build_primes.h"   /* void build_primes(idx_t limit, idx_t **out, size_t *count); */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */
#include "../utils/factorial.h"     /* seq_t factorial(idx_t n); */
#include "../utils/grow_seq.h"      /* void grow_seq(seq_t **arr, size_t *cap, size_t need); */

/* Cached sequence values and helper data. */
static seq_t  *seq_cache  = NULL;
static size_t   seq_len   = 0;          /* number of terms already cached */
static size_t   seq_cap   = 0;          /* current capacity of seq_cache */

static seq_t  digit_fact[10] = {0};     /* factorial of digits 0..9 */
static bool   fact_init = false;

/* Compute factorials of digits once. */
static void init_digit_fact(void)
{
    if (fact_init) return;
    for (int d = 0; d <= 9; ++d) {
        digit_fact[d] = factorial(d);
    }
    fact_init = true;
}

/* Sum the factorials of the decimal digits of a given unsigned integer. */
static seq_t digit_fact_sum(uint64_t x)
{
    seq_t s = 0;
    while (x > 0) {
        int d = (int)(x % 10);
        s += digit_fact[d];
        x /= 10;
    }
    return s;
}

/* Generate all terms from twin primes up to `limit`.
   This overwrites the current cache with the new list. */
static void generate_terms_up_to(idx_t limit)
{
    idx_t  *primes   = NULL;
    size_t  nprimes  = 0;

    init_digit_fact();

    /* Build list of primes up to the limit. */
    build_primes(limit, &primes, &nprimes);

    /* Allocate buffer for terms (worst case will be all primes). */
    size_t      new_cap = nprimes < 8 ? 8 : nprimes;
    seq_t     *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);   /* abort on allocation failure */

    size_t new_len = 0;

    /* Process twin prime pairs. */
    for (size_t i = 0; i + 1 < nprimes; ++i) {
        if (primes[i + 1] == primes[i] + 2) {
            seq_t sum1 = digit_fact_sum(primes[i]);
            seq_t sum2 = digit_fact_sum(primes[i + 1]);
            seq_t total = sum1 + sum2;

            if (is_prime_big(total)) {
                if (new_len >= new_cap) {
                    new_cap *= 2;
                    new_cache = (seq_t *)realloc(new_cache, new_cap * sizeof(seq_t));
                    if (!new_cache) exit(1);
                }
                new_cache[new_len++] = total;
            }
        }
    }

    /* Release old cache and primes array. */
    free(seq_cache);
    free(primes);

    seq_cache = new_cache;
    seq_len   = new_len;
    seq_cap   = new_cap;
}

/* Ensure that the cache contains at least `min_needed` terms. */
static void ensure_terms(size_t min_needed)
{
    static idx_t current_limit = 0;

    while (seq_len < min_needed) {
        /* Grow the search limit exponentially until enough terms are found. */
        if (current_limit == 0) current_limit = 1000000;   /* start with 1 million */
        else                    current_limit *= 2;

        generate_terms_up_to(current_limit);
    }
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n=0; sequence offset is 1 */
    size_t target = (size_t)n;
    ensure_terms(target);
    return seq_cache[n - 1];
}
