/*
 * A084792  Primes other than those of the form prime(prime(n)+n-1).
 * https://oeis.org/A084792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 2173, u32 n <= 2173, u64 n <= 2173,
 *            u128 n <= 2173, big any n
 * Helpers:   fenwick_find_kth
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A084792 by simulating the sieve:
 * start with the infinite list of primes;
 * at step k remove the prime(k)-th surviving prime,
 * obtaining the surviving primes as the sequence elements. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/fenwick_find_kth.h"

/* Assume seq_t and idx_t are defined by the build environment. */

static size_t prime_count;          /* total number of precomputed primes   */
static seq_t *primes;               /* array of all primes                 */
static int  *bit;                   /* Fenwick tree for alive flags         */
static size_t next_prime_to_add;    /* index of next prime to activate      */
static idx_t  alive_count;          /* number of primes currently alive    */
static idx_t  steps_done;           /* number of sieve steps performed     */
static bool    init_done = false;

/* Fenwick tree helpers ------------------------------------ */
static void fenwick_update(int *bit, size_t size, int idx, int delta) {
    for (int i = idx; i <= (int)size; i += i & -i) bit[i] += delta;
}

/* ----------------------------------------------------------- */

static void init(void) {
    /* Limit chosen so that the first 120 000 primes (prime(120000))
       are available – enough for n up to 10 000.          */
    const size_t LIMIT = 2000000UL;
    uint8_t *is_prime = (uint8_t *)malloc(LIMIT + 1);
    if (!is_prime) exit(1);
    memset(is_prime, 1, LIMIT + 1);
    is_prime[0] = is_prime[1] = 0;
    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i) is_prime[j] = 0;
        }
    }
    /* Count primes */
    for (size_t i = 2; i <= LIMIT; ++i) if (is_prime[i]) prime_count++;
    primes = (seq_t *)malloc(prime_count * sizeof(seq_t));
    if (!primes) exit(1);
    size_t j = 0;
    for (size_t i = 2; i <= LIMIT; ++i) if (is_prime[i]) primes[j++] = (seq_t)i;
    free(is_prime);
    /* Fenwick tree: use int as it will hold counts up to prime_count */
    bit = (int *)calloc(prime_count + 1, sizeof(int));
    if (!bit) exit(1);
    next_prime_to_add = 0;
    alive_count      = 0;
    steps_done        = 0;
    init_done = true;
}

seq_t a(idx_t n) {
    if (!init_done) init();
    if (n == 0) return 0;               /* offset 1 – value for n<1 is undefined */

    /* Run sieve steps until we have at least n alive primes. */
    while (alive_count < n) {
        idx_t step = steps_done + 1;             /* 1‑based step */
        seq_t p_seq = primes[step - 1];          /* prime(step) */
        idx_t p = (idx_t)p_seq;                 /* position to delete */

        /* Add primes until we have at least p alive primes. */
        while (alive_count < p) {
            if (next_prime_to_add >= prime_count) exit(1); /* safety */
            fenwick_update(bit, prime_count, (int)(next_prime_to_add + 1), 1);
            next_prime_to_add++;
            alive_count++;
        }

        /* Remove the p‑th alive prime. */
        int rem_pos = fenwick_find_kth(bit, prime_count, (int)p) - 1; /* 0‑based */
        fenwick_update(bit, prime_count, rem_pos + 1, -1);
        alive_count--;
        steps_done++;
    }

    /* Retrieve the n‑th alive prime. */
    int pos = fenwick_find_kth(bit, prime_count, (int)n) - 1; /* 0‑based */
    return primes[pos];
}
