/*
 * A063719  Numbers n such that usigma(cototient(n)) is a prime.
 * https://oeis.org/A063719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th number such that n - φ(n) + 1 is a prime
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   is_target
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* In this sequence,  n - φ(n) must equal 2,4,16,256 or 65536, i.e. a prime power whose +1 is a Fermat prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_target.h"

/* compute phi[0..limit] using a classic sieve */
static void compute_phi(uint64_t limit, uint64_t *phi)
{
    uint64_t i, j;
    /* initialise phi[i] = i */
    for (i = 0; i <= limit; ++i) phi[i] = i;

    /* sieve for primes */
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    for (i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (i = 2; i <= limit; ++i) {
        if (!is_prime[i]) continue;
        /* i is prime */
        phi[i] = i - 1;
        for (j = i * 2; j <= limit; j += i) {
            is_prime[j] = false;
            if (phi[j] == j) phi[j] = j; /* already set */
            phi[j] = phi[j] / i * (i - 1);
        }
    }
    free(is_prime);
}

/* append a value to the global sequence array */
static void append_seq(seq_t **seq, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        *cap = (*cap == 0) ? 16 : (*cap << 1);
        *seq = (seq_t *)realloc(*seq, (*cap) * sizeof(seq_t));
    }
    (*seq)[*len] = val;
    ++(*len);
}

/* the sequence function */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;

    static uint64_t *phi = NULL;
    static uint64_t phi_cap = 0;
    static idx_t searched_up_to = 0;   /* last n we have inspected */

    while (seq_len < n) {
        /* determine new limit for phi computation */
        idx_t new_limit = searched_up_to * 2;
        if (new_limit < searched_up_to + 1000000) new_limit = searched_up_to + 1000000;
        if (new_limit < searched_up_to + 100000) new_limit = searched_up_to + 100000; /* safety */
        if (new_limit <= searched_up_to) new_limit = searched_up_to + 100000; /* just in case */

        /* ensure phi array big enough */
        if (new_limit > phi_cap) {
            phi_cap = new_limit;
            phi = (uint64_t *)realloc(phi, (phi_cap + 1) * sizeof(uint64_t));
        }

        /* compute phi up to new_limit */
        compute_phi(new_limit, phi);

        /* examine numbers from searched_up_to+1 .. new_limit */
        for (idx_t i = searched_up_to + 1; i <= new_limit; ++i) {
            uint64_t cot = i - phi[i];
            if (is_target(cot))
                append_seq(&seq, &seq_len, &seq_cap, (seq_t)i);
        }

        searched_up_to = new_limit;
    }

    return seq[n - 1];
}
