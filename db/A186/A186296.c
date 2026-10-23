/*
 * A186296  ( A007520(n)+1 )/2.
 * https://oeis.org/A186296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p + 1)/2 where p is the nth prime congruent to 3 mod 8
 * Range:     u8 n <= 15, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A186296 */

#include <stdlib.h>

/* dynamic array to store primes congruent to 3 (mod 8) */
static seq_t *prime_arr = NULL;
static size_t prime_count = 0;
static size_t prime_cap = 0;

/* add a prime to the dynamic array */
static void add_prime(seq_t p)
{
    if (prime_count >= prime_cap) {
        size_t new_cap = (prime_cap == 0U ? 1024U : prime_cap << 1);
        seq_t *tmp = (seq_t *)realloc(prime_arr, new_cap * sizeof(seq_t));
        if (!tmp) { /* fatal error: cannot allocate more memory */
            exit(1);
        }
        prime_arr = tmp;
        prime_cap = new_cap;
    }
    prime_arr[prime_count++] = p;
}

/* generate primes up to a fixed limit and keep those ≡3 (mod 8) */
static void init_prime_list(void)
{
    const idx_t LIMIT = 1000000;   /* sufficient for n ≤ 10 000 */
    char *is_prime = (char *)malloc((size_t)(LIMIT + 1));
    if (!is_prime) {
        exit(1);
    }
    for (idx_t i = 0; i <= LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (idx_t p = 2; p * p <= LIMIT; ++p) {
        if (is_prime[p]) {
            for (idx_t q = p * p; q <= LIMIT; q += p) {
                is_prime[q] = 0;
            }
        }
    }

    for (idx_t n = 3; n <= LIMIT; n += 2) {          /* only odd candidates */
        if (is_prime[n] && (n & 7) == 3) {           /* n ≡ 3 (mod 8) */
            add_prime((seq_t)n);
        }
    }

    free(is_prime);
}

/* external function required by the test harness */
seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) {
        init_prime_list();
        initialized = 1;
    }

    if (n == 0) return 0;                /* undefined for n<1, safe guard */
    /* n is 1‑based; our array is 0‑based */
    seq_t p = prime_arr[(size_t)(n - 1)];
    return (p + 1) / 2;
}
