/*
 * A080361  a(n) is the difference between the largest and the smallest
 *          positive integers x such that the number of unitary-prime-divisors
 *          of x! equals n. Same as the difference between the largest and the
 *          smallest positive integers x such that the number of primes in
 *          (x/2,x] equals n.
 * https://oeis.org/A080361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = difference between the largest and the smallest positive
 *            integers x such that the number of primes in (x/2,x] is n.
 *            Implementation uses a sieve up to a fixed bound that
 *            (empirically) contains all occurrences for n up to at least 10⁵.
 *            The arrays min_x[n] and max_x[n] store the first and las...
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Forward declarations of idx_t and seq_t are supplied by the build. */
extern seq_t seq_from_str(const char *s);

static const idx_t LMAX = 20000000;          /* Upper bound for sieve */
static unsigned char *is_prime = NULL;      /* 0/1 array of size LMAX+1 */
static uint32_t *pi_arr = NULL;             /* Prefix prime counts */
static seq_t *min_x = NULL;                 /* Minimal x for each n */
static seq_t *max_x = NULL;                 /* Maximal x for each n */
static idx_t max_n_seen = 0;                /* Largest n encountered */
static bool initialized = false;

/* Simple sieve and precomputation */
static void init(void)
{
    size_t i, j;
    /* Allocate sieve */
    is_prime = (unsigned char*)calloc((size_t)LMAX + 1, 1);
    if (!is_prime) exit(EXIT_FAILURE);
    /* Mark all as prime initially */
    for (i = 2; i <= (size_t)LMAX; ++i) is_prime[i] = 1;
    /* Classical Eratosthenes */
    for (i = 2; i * i <= (size_t)LMAX; ++i) {
        if (is_prime[i]) {
            for (j = i * i; j <= (size_t)LMAX; j += i)
                is_prime[j] = 0;
        }
    }
    /* Pi array */
    pi_arr = (uint32_t*)malloc(((size_t)LMAX + 1) * sizeof(uint32_t));
    if (!pi_arr) exit(EXIT_FAILURE);
    pi_arr[0] = 0;
    for (i = 1; i <= (size_t)LMAX; ++i) {
        pi_arr[i] = pi_arr[i-1] + (uint32_t)is_prime[i];
    }
    /* Find maximum n encountered */
    max_n_seen = 0;
    for (i = 1; i <= (size_t)LMAX; ++i) {
        idx_t left = i >> 1;               /* floor(i/2) */
        idx_t g = pi_arr[i] - pi_arr[left];
        if (g > max_n_seen) max_n_seen = g;
    }
    /* Allocate min/max arrays */
    min_x = (seq_t*)calloc((size_t)max_n_seen + 1, sizeof(seq_t));
    max_x = (seq_t*)calloc((size_t)max_n_seen + 1, sizeof(seq_t));
    if (!min_x || !max_x) exit(EXIT_FAILURE);
    /* Fill min and max */
    for (i = 1; i <= (size_t)LMAX; ++i) {
        idx_t left = i >> 1;
        idx_t g = pi_arr[i] - pi_arr[left];
        if (g == 0) continue;
        if (min_x[g] == 0) min_x[g] = (seq_t)i;
        max_x[g] = (seq_t)i;
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init();
    if (n == 0) return 0;
    if (n > max_n_seen) return 0;
    /* If there was no x with this n (unlikely), return 0 */
    if (min_x[n] == 0) return 0;
    return max_x[n] - min_x[n];
}
