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
 * Range:     u8 none, u16 n <= 1, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-03)
 * Helpers:   build_primes, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>      /* size_t */
#include <stdint.h>      /* uint64_t */
#include <stdlib.h>      /* malloc, free */
#include <stdbool.h>     /* bool */

#include "../utils/build_primes.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

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
    seq_t     *new_cache = mem_new(seq_t, new_cap);
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
                    new_cache = mem_resize(seq_t, new_cache, new_cap);
                    if (!new_cache) exit(1);
                }
                new_cache[new_len++] = total;
            }
        }
    }

    /* Release old cache and primes array. */
    mem_free(seq_cache);
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
