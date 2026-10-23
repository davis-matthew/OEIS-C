/*
 * A045718  Nearest neighbors of primes.
 * https://oeis.org/A045718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Nearest neighbors of primes.
   a(n) is the n-th element (0‑based) of the sorted list of distinct integers
   of the form p−1 or p+1 for prime p.  The list is generated on demand. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Comparator for qsort over seq_t. */
static int cmp_seq_t(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* Global state: sorted unique neighbours and the current sieve limit. */
static seq_t *neighbors = NULL;          /* array of neighbours */
static size_t neighbors_len = 0;          /* number of stored terms     */
static size_t sieve_limit = 0;            /* largest prime processed    */

/* Extend the sieve and rebuild the neighbours list up to all primes ≤ new_limit. */
static void extend_sieve_to(size_t new_limit)
{
    if (new_limit <= sieve_limit)
        return;

    /* Simple sieve of Eratosthenes. */
    unsigned char *is_prime = (unsigned char *)malloc(new_limit + 1);
    if (!is_prime)
        exit(1);

    memset(is_prime, 1, new_limit + 1);
    if (new_limit >= 0) is_prime[0] = 0;
    if (new_limit >= 1) is_prime[1] = 0;
    for (size_t i = 2; i * i <= new_limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= new_limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count how many neighbour values fit into seq_t. */
    size_t val_limit;
    if (sizeof(seq_t) <= sizeof(size_t))
        /* Equivalent to (size_t)((seq_t)-1) but portable. */
        val_limit = (size_t)( (seq_t)-1 );
    else
        val_limit = SIZE_MAX;

    size_t max_val = new_limit + 1;          /* largest possible neighbour */
    size_t count = 0;
    for (size_t p = 2; p <= new_limit; ++p) {
        if (is_prime[p]) {
            if (p - 1 <= val_limit) ++count;
            if (p + 1 <= val_limit) ++count;
        }
    }

    /* Collect neighbour values. */
    seq_t *raw = (seq_t *)malloc(count * sizeof(seq_t));
    if (!raw) exit(1);

    size_t idx = 0;
    for (size_t p = 2; p <= new_limit; ++p) {
        if (is_prime[p]) {
            if (p - 1 <= val_limit) raw[idx++] = (seq_t)(p - 1);
            if (p + 1 <= val_limit) raw[idx++] = (seq_t)(p + 1);
        }
    }
    free(is_prime);

    /* Sort and deduplicate. */
    if (count > 0) {
        qsort(raw, count, sizeof(seq_t), cmp_seq_t);
    }
    size_t unique = 0;
    if (count > 0) {
        seq_t prev = raw[0];
        raw[unique++] = prev;
        for (size_t i = 1; i < count; ++i) {
            if (raw[i] != prev) {
                prev = raw[i];
                raw[unique++] = prev;
            }
        }
    }

    /* Allocate final array. */
    seq_t *final = (seq_t *)malloc(unique * sizeof(seq_t));
    if (!final) exit(1);
    memcpy(final, raw, unique * sizeof(seq_t));

    free(raw);
    free(neighbors);
    neighbors = final;
    neighbors_len = unique;
    sieve_limit = new_limit;
}

/* Public accessor required by the build system. */
seq_t a(idx_t n)
{
    /* Grow the list until it contains the n-th element. */
    while (neighbors_len <= (size_t)n) {
        size_t new_limit = (sieve_limit == 0) ? 30 : sieve_limit * 2;
        extend_sieve_to(new_limit);
    }
    return neighbors[(size_t)n];
}
