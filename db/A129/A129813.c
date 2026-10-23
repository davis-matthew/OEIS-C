/*
 * A129813  Lesser of consecutive prime pairs whose last digits differ by 8.
 * https://oeis.org/A129813
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   grow_seq_array, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n): lesser of consecutive prime pairs whose last digits differ by 8
   (|p%10 – q%10| = 8), offset 2.
   The function keeps a static list of found terms so calls can be in any order. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/grow_seq_array.h"

seq_t a(idx_t n)
{
    /* Invalid indices return 0; sequence only defined for n >= 2. */
    if (n < 2)
        return (seq_t)0;

    /* Convert to 0‑based index into the internal list. */
    const idx_t target = n - 2;

    /* Static cache of results. */
    static seq_t *list = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    /* Static state for the prime search. */
    static uint64_t p = 0;   /* current prime */
    static uint64_t q = 0;   /* next prime after p */

    /* Initialise the first primes on the first call. */
    if (p == 0) {
        p = 2;
        q = next_prime(p);
    }

    /* Generate terms until we have at least target+1 elements. */
    while (len <= target) {
        /* Scan consecutive primes until a qualifying pair is found. */
        while (1) {
            const uint64_t p10 = p % 10;
            const uint64_t q10 = q % 10;

            /* A pair qualifies if the last digits differ by 8. */
            if ((p10 == 9 && q10 == 1) || (p10 == 1 && q10 == 9)) {
                /* Append the smaller prime. */
                grow_seq_array(&list, &cap, len + 1);
                list[len++] = (seq_t)p;

                /* Move past this pair for the next search. */
                p = q;
                q = next_prime(p);
                break;  /* inner loop: continue with next pair search. */
            }

            /* Not a match: advance to the next consecutive pair. */
            p = q;
            q = next_prime(p);
        }
    }

    return list[target];
}
