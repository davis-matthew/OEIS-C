/*
 * A066246  a(n) = 0 unless n is a composite number A002808(k) then a(n) = k.
 * https://oeis.org/A066246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n is not a composite number; otherwise * a(n) = the
 *            index of n among the composite numbers (starting with 4 as *
 *            index 1). Example: a(4)=1, a(6)=2, a(9)=4, a(10)=5
 * Range:     u8 n <= 322, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation note: a sieve of Eratosthenes is kept in static
 *  state and extended lazily to cover the largest n requested.  The
 *  cumulative count of composites up to each index is stored so that
 *  a(n) can be returned in O(1) after the initial sieve.  A
 *  C/C++ compatible cast is used for malloc results so the same
 *  source compiles in both C11 and C++17.  */

/*  The sequence requires that a(idx_t n) be defined; seq_t is an
 *  arbitrary‑precision integer type or a native unsigned type provided
 *  by the build system.  All arithmetic is performed with compatible
 *  integer types.  */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static size_t current_limit = 0;          /* largest n currently sieved */
static uint8_t *prime_flags = NULL;       /* 1 = prime (for i>=2), 0 otherwise */
static uint64_t *cum_comp = NULL;         /* cumulative composite count */

/*  Extend the sieve up to at least new_limit (inclusive).  If the
 *  array already covers this range, do nothing.  */
static void extend_sieve(size_t new_limit)
{
    if (new_limit <= current_limit)
        return;

    /*  allocate new arrays */
    uint8_t  *new_flags = (uint8_t *)malloc((new_limit + 1) * sizeof(uint8_t));
    uint64_t *new_cum   = (uint64_t *)malloc((new_limit + 1) * sizeof(uint64_t));
    if (!new_flags || !new_cum) {
        /* abort on OOM – the sequence build is expected to run in
         * a controlled environment, so exit is acceptable.  */
        exit(EXIT_FAILURE);
    }

    /*  initialise all as prime (1); 0 and 1 are handled separately  */
    for (size_t i = 2; i <= new_limit; ++i)
        new_flags[i] = 1;
    new_flags[0] = new_flags[1] = 0;

    /*  Sieve of Eratosthenes  */
    for (size_t i = 2; i * i <= new_limit; ++i) {
        if (new_flags[i]) {
            for (size_t j = i * i; j <= new_limit; j += i)
                new_flags[j] = 0;
        }
    }

    /*  cumulative composite count  */
    new_cum[0] = 0;
    new_cum[1] = 0;
    for (size_t i = 2; i <= new_limit; ++i) {
        new_cum[i] = new_cum[i - 1];
        if (i >= 4 && new_flags[i] == 0)   /* i is composite */
            ++new_cum[i];
    }

    /*  replace old arrays  */
    free(prime_flags);
    free(cum_comp);
    prime_flags = new_flags;
    cum_comp    = new_cum;
    current_limit = new_limit;
}

seq_t a(idx_t n)
{
    size_t nn = (size_t)n;
    if (nn < 4)          /* 1,2,3 are never composites */
        return (seq_t)0;

    extend_sieve(nn);

    if (prime_flags[nn] == 0)    /* composite */
        return (seq_t)cum_comp[nn];
    else
        return (seq_t)0;
}
