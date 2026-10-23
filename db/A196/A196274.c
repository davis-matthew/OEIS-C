/*
 * A196274  Half of the gaps A067970 between odd nonprimes A014076.
 * https://oeis.org/A196274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A014076(n+1) - A014076(n)) / 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A014076 lists the odd non‑prime numbers: 1,9,15,21,25,27,33,… */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */

/* Static storage for the odd non‑prime sequence A014076. */
static uint64_t *odd_nonprimes = NULL;
static size_t odd_count = 0;     /* number of elements stored          */
static size_t odd_capacity = 0;  /* allocated capacity                  */

/* Ensure storage holds at least 'needed' elements. */
static void ensure_odd_capacity(size_t needed)
{
    if (needed <= odd_capacity)
        return;

    size_t newcap = odd_capacity ? odd_capacity * 2 : 64;
    while (newcap < needed)
        newcap *= 2;

    uint64_t *tmp = (uint64_t *)realloc(odd_nonprimes, newcap * sizeof(uint64_t));
    if (!tmp && newcap > 0) {
        /* Allocation failed – abort to avoid undefined behaviour. */
        abort();
    }
    odd_nonprimes = tmp;
    odd_capacity = newcap;
}

/* Initialise the array with the first odd non‑prime, 1. */
static void init_odd_nonprimes(void)
{
    if (odd_nonprimes == NULL) {
        odd_nonprimes = (uint64_t *)malloc(64 * sizeof(uint64_t));
        if (!odd_nonprimes)
            abort();
        odd_nonprimes[0] = (uint64_t)1;
        odd_count = 1;
        odd_capacity = 64;
    }
}

/* Return a(n) for n >= 1.  Undefined behaviour for n < 1. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* guard for out of range */

    init_odd_nonprimes();

    /* Generate odd non‑primes until we have at least n+1 of them. */
    while (odd_count <= (size_t)n) {
        uint64_t candidate = odd_nonprimes[odd_count - 1] + (uint64_t)2;
        for (;;) {
            if (!is_prime(candidate)) {
                ensure_odd_capacity(odd_count + 1);
                odd_nonprimes[odd_count++] = candidate;
                break;
            }
            candidate += (uint64_t)2;    /* skip to next odd number */
        }
    }

    /* Compute half of the gap between successive odd non‑primes. */
    uint64_t gap = odd_nonprimes[(size_t)n] - odd_nonprimes[(size_t)(n - 1)];
    return (seq_t)(gap / (uint64_t)2);
}
