/*
 * A052284  Number of compositions of n into nonprime numbers.
 * https://oeis.org/A052284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of compositions of n into nonprime numbers a(n) =
 *            Σ_{d=1..n, d not prime} a(n-d), a(0) = 1
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 54, u64 n <= 107,
 *            u128 n <= 211, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --- Dependencies --------------------------------------------------- */
#include <stdlib.h>
#include <stdbool.h>

/* --- Static state ---------------------------------------------------- */
static seq_t *A = NULL;          /* a(n) values */
static seq_t *S = NULL;          /* cumulative sums: S[i] = Σ_{k=0}^{i} a(k) */
static size_t capacity = 0;      /* allocated capacity for A and S */
static idx_t next_to_compute = 0;/* first index not yet computed */

/* Prime handling */
static idx_t *prime_list = NULL; /* primes ≤ prime_limit */
static size_t prime_count = 0;
static idx_t prime_limit = 0;    /* current maximum number for which primes are known */

/* --- Helper functions ------------------------------------------------ */
static void ensure_capacity(idx_t needed)
{
    size_t needed_cap = (size_t)needed + 1;
    if (capacity >= needed_cap)
        return;
    size_t new_cap = capacity ? capacity << 1 : 16;
    while (new_cap < needed_cap)
        new_cap <<= 1;
    seq_t *tmpA = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
    seq_t *tmpS = (seq_t *)realloc(S, new_cap * sizeof(seq_t));
    if (!tmpA || !tmpS) /* memory allocation failed */
        abort();
    A = tmpA;
    S = tmpS;
    capacity = new_cap;
}

static void regenerate_primes_upto(idx_t limit)
{
    bool *sieve = (bool *)calloc((size_t)limit + 1, sizeof(bool));
    if (!sieve)
        abort();
    for (idx_t i = 2; i <= limit; ++i)
        sieve[i] = true;

    for (idx_t i = 2; (idx_t)((long long)i * i) <= limit; ++i) {
        if (sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                sieve[j] = false;
        }
    }

    size_t count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (sieve[i])
            ++count;

    idx_t *new_list = count ? (idx_t *)malloc(count * sizeof(idx_t)) : NULL;
    if (!new_list && count)
        abort();

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (sieve[i])
            new_list[idx++] = i;
    }

    free(prime_list);
    prime_list = new_list;
    prime_count = count;
    prime_limit = limit;
    free(sieve);
}

static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;
    regenerate_primes_upto(limit);
}

/* --- Sequence function ------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Initialise static storage on first call */
    if (A == NULL) {
        ensure_capacity(0);
        /* Base case */
        A[0] = (seq_t)1;
        S[0] = (seq_t)1;
        next_to_compute = 1;
    }

    /* Ensure arrays are large enough for requested n */
    ensure_capacity(n);

    if (n >= next_to_compute) {
        /* Make sure primes up to n are available */
        ensure_primes(n);

        /* Compute all missing terms up to n */
        for (idx_t i = next_to_compute; i <= n; ++i) {
            /* Sum of all a(k) for k < i */
            seq_t sum = S[i - 1];

            /* Subtract contributions from prime parts */
            for (size_t j = 0; j < prime_count && prime_list[j] <= i; ++j)
                sum -= A[i - prime_list[j]];

            A[i] = sum;
            S[i] = S[i - 1] + sum;
        }
        next_to_compute = n + 1;
    }

    return A[n];
}
