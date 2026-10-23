/*
 * A105365  Rearrangement of positive integers according to parity of
 *          (prime(n)-1)/2 (see comments).
 * https://oeis.org/A105365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   uses the pattern parity of (prime(n)-1)/2 * to output the next
 *            unused prime or composite number
 * Range:     u8 n <= 58, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static storage for the sequence values, prime list and
 * prime/composite state. All indices are 1‑based. */
static seq_t *a_vals = NULL;           /* a_values[1..] */
static size_t a_cap = 0;               /* allocated capacity of a_vals */
static idx_t a_size = 0;               /* highest index computed */

static seq_t *primes = NULL;           /* primes[1..] */
static size_t primes_cap = 0;          /* allocated capacity of primes */
static idx_t primes_count = 0;         /* number of primes stored */

/* The next candidate for the prime generation */
static seq_t prime_candidate = 2;
/* The next index in the primes array to be output as a prime */
static idx_t next_prime_output_index = 1;
/* The smallest integer to test for the next composite output */
static seq_t composite_candidate = 4;

/* ------------------------------------------------------------ */
/* Utility: ensure capacity of an array of seq_t values.        */
static void ensure_capacity(size_t *cap, seq_t **arr, size_t needed)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap) ? (*cap) * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(*arr, (new_cap + 1) * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);      /* out of memory */
    *arr = tmp;
    *cap = new_cap;
}

/* ------------------------------------------------------------ */
/* Test whether n (stored in seq_t) is prime using trial division
 * with all primes already known. */
static bool is_prime_seq(seq_t n)
{
    if (n < 2) return false;
    for (idx_t i = 1; i <= primes_count; ++i) {
        seq_t p = primes[i];
        seq_t sq = p * p;
        if (sq > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------ */
/* Generate primes until we have 'target' primes stored. */
static void generate_primes_upto(idx_t target)
{
    while (primes_count < target) {
        while (!is_prime_seq(prime_candidate))
            ++prime_candidate;              /* skip non‑primes */
        /* store the new prime */
        if (primes_count + 1 > primes_cap)
            ensure_capacity(&primes_cap, &primes, primes_count + 1);
        primes[++primes_count] = prime_candidate;
        ++prime_candidate;
    }
}

/* ------------------------------------------------------------ */
/* Compute a(n) for the OEIS sequence A105365. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n<1 */

    /* Grow the array of sequence values if necessary */
    ensure_capacity(&a_cap, &a_vals, n);
    while (a_size < n) {
        ++a_size;                 /* next index to compute (1‑based) */

        if (a_size == 1) {
            a_vals[1] = 1;        /* a(1)=1 */
            continue;
        }

        /* Ensure we know the n‑th prime for parity test. */
        generate_primes_upto(a_size);
        seq_t pn = primes[a_size];           /* prime(n) */

        /* Parity of (prime(n)-1)/2 */
        seq_t parity = (pn - 1) / 2;
        bool even = ((parity & 1) == 0);     /* even ↔ next prime */

        if (even) {
            /* Output the next unused prime. */
            a_vals[a_size] = primes[next_prime_output_index++];
        } else {
            /* Output the next unused composite. */
            while (is_prime_seq(composite_candidate))
                ++composite_candidate;
            a_vals[a_size] = composite_candidate;
            ++composite_candidate;
        }
    }
    return a_vals[n];
}
