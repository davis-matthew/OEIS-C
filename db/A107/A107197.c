/*
 * A107197  Primes of the form 3x^2 + 28y^2.
 * https://oeis.org/A107197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 42, u32 n <= 363, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isprime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A107197:
   primes of the form 3*x^2 + 28*y^2 (x,y ≥ 0, x>0), ordered increasing. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isprime_big.h"

static seq_t *prime_arr = NULL;     /* stored primes */
static idx_t  prime_count = 0;      /* how many primes we have */
static idx_t  prime_cap   = 0;      /* allocated capacity */

/* push a new prime into prime_arr, reallocating if necessary */
static void push_prime(seq_t val)
{
    if (prime_count == prime_cap) {
        prime_cap = prime_cap ? prime_cap * 2 : 16;
        prime_arr = (seq_t *)realloc(prime_arr, prime_cap * sizeof(seq_t));
    }
    prime_arr[prime_count++] = val;
}

/* generate all values 3*x^2 + 28*y^2 in the interval (old_bound, new_bound]
   that are prime, and add them to prime_arr. */
static void generate_range(seq_t old_bound, seq_t new_bound)
{
    /* iterate over y ≥ 0 */
    for (idx_t y = 0; ; ++y) {
        __int128 ypart128 = (__int128)28 * y * y;
        if (ypart128 > ( __int128)new_bound) break;
        seq_t ypart = (seq_t)ypart128;

        /* iterate over x ≥ 1 */
        for (idx_t x = 1; ; ++x) {
            __int128 val128 = (__int128)3 * x * x + ypart128;
            if (val128 > ( __int128)new_bound) break;
            seq_t val = (seq_t)val128;
            if (val > old_bound && isprime_big(val))
                push_prime(val);
        }
    }
}

/* comparison function for qsort */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *aa = (const seq_t *)a;
    const seq_t *bb = (const seq_t *)b;
    if (*aa < *bb) return -1;
    if (*aa > *bb) return  1;
    return 0;
}

/* remove duplicates after sorting */
static void unique_primes(void)
{
    if (prime_count == 0) return;
    idx_t write = 1;
    for (idx_t i = 1; i < prime_count; ++i) {
        if (prime_arr[i] != prime_arr[write - 1])
            prime_arr[write++] = prime_arr[i];
    }
    prime_count = write;
}

/* ensure that prime_arr holds at least n primes, sorted and unique.
   We expand the search bound until we have enough primes. */
static void ensure_size(idx_t n)
{
    static seq_t last_checked_bound = 0;
    static bool initialized = false;
    if (!initialized) {
        last_checked_bound = 0;
        initialized = true;
    }

    seq_t bound = (seq_t)1000;                /* start with a modest bound */
    while (prime_count < n) {
        generate_range(last_checked_bound, bound);
        qsort(prime_arr, prime_count, sizeof(seq_t), cmp_seq);
        unique_primes();

        last_checked_bound = bound;
        bound = bound * 2 + (seq_t)1000;      /* increase bound for next round */
    }

    /* final check in case the last round didn't produce enough primes */
    if (prime_count < n) {
        generate_range(last_checked_bound, bound);
        qsort(prime_arr, prime_count, sizeof(seq_t), cmp_seq);
        unique_primes();
        last_checked_bound = bound;
    }
}

/* required public function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* not used; may return 0 for safety */
    ensure_size(n);
    return prime_arr[n - 1];
}
