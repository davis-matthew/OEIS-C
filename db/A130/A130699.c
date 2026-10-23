/*
 * A130699  Numbers n for which neither 2n-3 nor 2n+3 are primes.
 * https://oeis.org/A130699
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A130699: compute the n-th integer n such that neither 2n-3 nor 2n+3 are primes.
   The sequence starts with a(2)=6, a(3)=9, … (offset 2). This code builds a list of
   such numbers on demand and returns the requested term. */
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 2; values below the offset are undefined. */
    if (n < 2)
        return (seq_t)0; /* arbitrary for out-of-range indices */

    static seq_t *list = NULL;       /* list of terms (a(2), a(3), …) */
    static size_t count = 0;         /* number of terms stored */
    static size_t capacity = 0;      /* allocated capacity for list */
    static idx_t last_candidate = 2; /* next candidate value to examine */

    /* Generate terms until we have at least n terms. */
    while (count < n)
    {
        /* Check primes 2*last_candidate-3 and 2*last_candidate+3. */
        idx_t val1 = 2 * last_candidate - 3;
        idx_t val2 = 2 * last_candidate + 3;
        bool p1 = is_prime((idx_t)(val1 >= 2 ? val1 : 0));
        bool p2 = is_prime((idx_t)(val2 >= 2 ? val2 : 0));

        if (!p1 && !p2)
        {
            /* Need space for an additional element. */
            grow_seq(&list, &capacity, count + 1);
            list[count++] = (seq_t)last_candidate;
        }
        ++last_candidate;
    }

    /* The first stored element corresponds to n = 2, so adjust index. */
    return list[n - 2];
}
