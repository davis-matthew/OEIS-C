/*
 * A085756  Number of partitions into a prime number of distinct parts.
 * https://oeis.org/A085756
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   Number of partitions of n into a prime number of distinct parts
 *            * Using the transformation that partitions of n into p distinct
 *            parts correspond * to partitions of n - p(p-1)/2 into p
 *            (unrestricted) parts
 * Range:     u8 n <= 33, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

static seq_t *part_table = NULL;          /* flattened 2D table: part[k][n] */
static size_t global_cols = 0;            /* number of n columns in table */
static idx_t global_max_n = 0;            /* maximum n that has been processed */
static size_t global_Kmax = 0;            /* maximum k (prime part count) processed */
static int *prime_sieve = NULL;           /* is_prime array up to global_Kmax */
static size_t prime_sieve_size = 0;

/* Recompute the DP tables up to N and primes up to Kmax */
static void recompute(idx_t N)
{
    if (N <= global_max_n) return; /* already up to date */

    /* compute maximum number of parts needed for any n <= N */
    size_t new_Kmax = (size_t)((sqrt(8.0 * (double)N + 1) - 1) / 2);
    size_t rows = new_Kmax + 1;
    size_t cols = (size_t)N + 1;
    global_cols = cols;

    /* allocate new table and zero it */
    seq_t *new_table = (seq_t*)calloc(rows * cols, sizeof(seq_t));
    if (!new_table) return; /* out of memory – caller will get 0 */

    /* base case: partition of 0 into 0 parts is 1 */
    new_table[0 * cols + 0] = 1;

    /* DP: p(n,k) = p(n-1,k-1) + p(n-k,k) */
    for (size_t n = 1; n <= N; ++n) {
        for (size_t k = 1; k <= new_Kmax; ++k) {
            if (n < k) continue; /* remains 0 */
            seq_t a = new_table[(k-1) * cols + (n-1)];
            seq_t b = new_table[k * cols + (n - k)];
            new_table[k * cols + n] = a + b;
        }
    }

    /* free old table and replace */
    free(part_table);
    part_table = new_table;
    global_Kmax = new_Kmax;
    global_max_n = N;

    /* compute primes up to new_Kmax */
    if (prime_sieve_size < global_Kmax + 1) {
        free(prime_sieve);
        prime_sieve = (int*)calloc(global_Kmax + 1, sizeof(int));
        prime_sieve_size = global_Kmax + 1;
    }
    for (size_t i = 0; i <= global_Kmax; ++i) prime_sieve[i] = 1;
    if (global_Kmax >= 0) {
        prime_sieve[0] = 0;
        if (global_Kmax >= 1) prime_sieve[1] = 0;
    }
    for (size_t i = 2; i * i <= global_Kmax; ++i) {
        if (!prime_sieve[i]) continue;
        for (size_t j = i * i; j <= global_Kmax; j += i) {
            prime_sieve[j] = 0;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;        /* offset is 3 */
    recompute(n);

    seq_t result = 0;
    size_t cols = global_cols;

    for (size_t p = 2; p <= global_Kmax; ++p) {
        if (!prime_sieve[p]) continue;
        idx_t min_sum = (idx_t)p * (p + 1) / 2;   /* minimal n for p distinct parts */
        if (min_sum > n) continue;
        idx_t M = n - (idx_t)p * (p - 1) / 2;      /* map to unrestricted partitions */
        result += part_table[p * cols + M];
    }
    return result;
}
