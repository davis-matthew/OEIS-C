/*
 * A204900  Least k such that n divides s(k)-s(j) for some j in [1,k), where
 *          s(k) is the k-th odd prime.
 * https://oeis.org/A204900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal k such that there exist j < k with s(k) ≡ s(j)
 *            (mod n), where s(k) is the k-th odd prime (3,5,7,11,…). The
 *            implementation achieves this by generating odd primes
 *            incrementally and detecting the first pair of primes yielding
 *            equal residues modulo n
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static data for prime generation – they are retained across calls. */
static seq_t *odd_primes = NULL;          /* array of odd primes (s(k))   */
static size_t odd_count  = 0;
static size_t odd_cap    = 0;

static seq_t *primes_all = NULL;          /* all primes, including 2 */
static size_t prime_all_count = 0;
static size_t prime_all_cap    = 0;

static idx_t next_candidate = 3;          /* next odd number to test */

/* Grow an array of seq_t when needed. */
static void grow_seq_arr(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);                /* out of memory */
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Ensure that at least ‘required’ odd primes have been generated. */
static void ensure_odd_primes(size_t required)
{
    /* Ensure storage for odd primes */
    grow_seq_arr(&odd_primes, &odd_cap, required);

    /* Ensure storage for all primes (used for primality tests) */
    grow_seq_arr(&primes_all, &prime_all_cap, required + 1);

    /* If this is the first time, initialise the prime list with 2. */
    if (prime_all_count == 0) {
        primes_all[prime_all_count++] = (seq_t)2U;
    }

    while (odd_count < required) {
        /* Test candidate for primality using stored primes. */
        bool is_prime = true;
        for (size_t i = 0; i < prime_all_count; ++i) {
            idx_t p = (idx_t)primes_all[i];
            if (p * p > next_candidate) break;
            if ((next_candidate % p) == 0) {
                is_prime = false;
                break;
            }
        }

        /* Advance to next odd number for future iterations. */
        next_candidate += 2;

        if (is_prime) {
            /* Store the new prime. */
            primes_all[prime_all_count++] = (seq_t)next_candidate - 2; /* previous value was used */
            odd_primes[odd_count++] = (seq_t)next_candidate - 2;
        }
    }
}

/* Function to compute a(n). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* undefined, but safe guard */
    /* We need at least n+1 odd primes to guarantee the pigeonhole principle. */
    ensure_odd_primes((size_t)n + 1);

    /* Allocate a small boolean array indexed by residues modulo n. */
    bool *seen = (bool*)malloc((size_t)n * sizeof(bool));
    if (!seen) exit(1);
    for (size_t i = 0; i < (size_t)n; ++i) seen[i] = false;

    seq_t result = 0;
    /* Scan through the odd primes until the first repeated residue is found. */
    for (size_t i = 0; i < odd_count; ++i) {
        /* Convert prime to idx_t for modulo operation (all primes are small). */
        idx_t p = (idx_t)odd_primes[i];
        idx_t r = p % n;
        if (seen[r]) {
            result = (seq_t)(i + 1);  /* k is 1‑based index of s(k) */
            break;
        }
        seen[r] = true;
    }

    free(seen);
    return result;
}
