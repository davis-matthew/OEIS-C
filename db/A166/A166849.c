/*
 * A166849  Primes with digital root 1, 2, 4 or 8.
 * https://oeis.org/A166849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th prime whose digital root is 1, 2, 4, or 8
 * Range:     u8 n <= 36, u16 n <= 4360, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Grow an array of seq_t elements. */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
    while (new_cap < need) new_cap <<= 1;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* Grow an array of uint64_t primes. */
static void grow_primes(uint64_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t new_cap = (*cap == 0) ? 128 : (*cap << 1);
    while (new_cap < need) new_cap <<= 1;
    *arr = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
    *cap = new_cap;
}

/* Return the n-th term of sequence A166849. */
seq_t a(idx_t n)
{
    /* Allowed residues modulo 9 for digital roots 1,2,4,8. */
    static const bool allow[9] = {
        false, /* 0 */
        true,  /* 1 */
        true,  /* 2 */
        false, /* 3 */
        true,  /* 4 */
        false, /* 5 */
        false, /* 6 */
        false, /* 7 */
        true   /* 8 */
    };

    /* Static state of the generated sequence. */
    static seq_t *seq = NULL;
    static size_t seq_len = 0, seq_cap = 0;

    /* Primes used for trial division. */
    static uint64_t *primes = NULL;
    static size_t p_len = 0, p_cap = 0;

    /* Next integer to test for primality. */
    static uint64_t candidate = 1;

    /* Convert index to a size_t. */
    size_t target = (size_t)n;
    if (target == 0) return (seq_t)0;          /* offset = 1, guard */

    /* Generate until we have at least target terms. */
    while (seq_len < target)
    {
        /* Find next prime. */
        do
        {
            ++candidate;

            /* Quick reject: even numbers > 2. */
            if (candidate > 2 && (candidate & 1ULL) == 0ULL)
                continue;

            bool prime = true;

            /* Trial division with known primes. */
            for (size_t i = 0; i < p_len; ++i)
            {
                uint64_t p = primes[i];
                if (p > candidate / p)     /* p*p > candidate */
                    break;
                if (candidate % p == 0ULL)
                {
                    prime = false;
                    break;
                }
            }

            if (!prime)
                continue;

            /* Store the new prime. */
            grow_primes(&primes, &p_cap, p_len + 1);
            primes[p_len++] = candidate;

            /* Check digital‑root condition. */
            if (allow[candidate % 9])
            {
                grow_seq(&seq, &seq_cap, seq_len + 1);
                seq[seq_len++] = (seq_t)candidate;
            }

        } while (seq_len < target);
    }

    return seq[target - 1];
}
