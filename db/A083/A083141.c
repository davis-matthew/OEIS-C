/*
 * A083141  Main diagonal of array in A083140.
 * https://oeis.org/A083141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) * prime(2*n-2) (a(1)=2)
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Prime table (initialized once) ---------- */
static int *prime_table   = NULL;   /* 0‑based array of primes */
static size_t prime_count = 0;      /* number of primes stored */

/* Build a simple sieve up to a bound that guarantees >20000 primes
   (the 19998‑th prime is < 250000).  The sieve is run only once. */
static void init_prime_table(void)
{
    if (prime_table != NULL) return;        /* already built */

    const size_t LIMIT = 300000;            /* enough for our needs */
    /* Allocate a boolean sieve; cast because C++ forbids implicit conversion. */
    bool *sieve = (bool*)calloc(LIMIT + 1, sizeof(bool));
    if (sieve == NULL) return;             /* allocation failed */

    size_t count = 0;
    /* First pass: count how many primes there will be. */
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (!sieve[i]) {
            ++count;
        }
        if (i * i <= LIMIT) {
            for (size_t j = i * i; j <= LIMIT; j += i) {
                sieve[j] = true;
            }
        }
    }

    /* Allocate array for primes. */
    prime_table = (int*)malloc(count * sizeof(int));
    if (prime_table == NULL) {
        free(sieve);
        return;
    }

    /* Second pass: collect the primes. */
    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT && idx < count; ++i) {
        if (!sieve[i]) {
            prime_table[idx++] = (int)i;
        }
    }
    prime_count = count;
    free(sieve);
}

/* Return the nth prime (1‑based).  The caller guarantees n >= 1. */
static inline int get_prime_by_index(idx_t n)
{
    return prime_table[n - 1];
}

/* ---------- Sequence implementation ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                  /* offset starts at 1 */

    init_prime_table();                   /* lazily build the prime array */

    if (n == 1)                            /* special case: prime(0) is taken as 1 */
        return (seq_t)2;

    /* For n >= 2, both indices are >= 2 and <= 19998 for n <= 10000. */
    idx_t idx_n  = n;
    idx_t idx_2n = 2 * n - 2;

    int p1 = get_prime_by_index(idx_n);
    int p2 = get_prime_by_index(idx_2n);

    return (seq_t)p1 * (seq_t)p2;
}
