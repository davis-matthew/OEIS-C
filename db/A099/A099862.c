/*
 * A099862  a(n) = (2*k)-th composite number; a bisection of A002808.
 * https://oeis.org/A099862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the (2*n)-th composite number
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static storage for composites and primes. */
static seq_t *composites = NULL;      /* array of composite seq_t values   */
static size_t composites_size = 0;    /* number of composites stored       */
static size_t composites_cap   = 0;    /* allocated capacity of composites  */

static idx_t *primes = NULL;          /* array of primes (idx_t values)     */
static size_t primes_size = 0;        /* number of primes stored            */
static size_t primes_cap   = 0;        /* allocated capacity of primes       */

static idx_t last_candidate = 3;      /* last integer examined               */

/* Grow a dynamic array of idx_t. */
static void grow_idx_array(idx_t **arr, size_t *size, size_t *cap, size_t new_cnt)
{
    if (*size + new_cnt > *cap) {
        size_t new_cap = (*cap == 0) ? 256 : *cap * 2;
        while (new_cap < *size + new_cnt) new_cap *= 2;
        *arr = (idx_t *)realloc(*arr, new_cap * sizeof(idx_t));
        *cap = new_cap;
        if (*arr == NULL) exit(1);   /* allocation failed */
    }
}

/* Grow composites array. */
static void grow_composites(size_t new_cnt)
{
    if (composites_size + new_cnt > composites_cap) {
        size_t new_cap = (composites_cap == 0) ? 1024 : composites_cap * 2;
        while (new_cap < composites_size + new_cnt) new_cap *= 2;
        composites = (seq_t *)realloc(composites, new_cap * sizeof(seq_t));
        composites_cap = new_cap;
        if (composites == NULL) exit(1);
    }
}

/* Ensure that at least 'target' composite numbers (1‑based) are generated. */
static void ensure_composites(idx_t target)
{
    /* Initialise the primes list with the smallest primes if empty. */
    if (primes_size == 0) {
        grow_idx_array(&primes, &primes_size, &primes_cap, 2);
        primes[primes_size++] = 2;
        primes[primes_size++] = 3;
    }

    while (composites_size < target) {
        idx_t cand = last_candidate + 1;
        last_candidate = cand;

        bool is_prime = true;
        for (size_t i = 0; i < primes_size; ++i) {
            idx_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* Store new prime. */
            grow_idx_array(&primes, &primes_size, &primes_cap, 1);
            primes[primes_size++] = cand;
        } else {
            /* Store new composite. */
            grow_composites(1);
            composites[composites_size++] = (seq_t)cand;
        }
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* undefined for offset 1 */
    idx_t target = 2 * n;    /* need at least (2*n) composites */
    ensure_composites(target);
    return composites[(size_t)(2 * n - 1)];
}
