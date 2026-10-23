/*
 * A100807  Primes p such that either (or both) p-10 or p+10 are primes.
 * https://oeis.org/A100807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A100807: primes p such that either p-10 or p+10 is prime
   (OEIS offset 1 – a(1) = 3).
   The function searches primes incrementally, keeping a memoised list
   so that any order of calls is handled efficiently. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"          /* provides bool is_prime(uint64_t) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* behaviour for n < 1 is irrelevant */

    /* memoised list of sequence values */
    static seq_t *list = NULL;
    static idx_t count = 0;           /* number of terms stored */
    static size_t cap    = 0;         /* current allocation capacity */
    static uint64_t candidate = 2;    /* next integer to test */

    /* Grow the list until it contains at least n terms */
    while (count < n) {
        /* Find the next prime satisfying the ±10 condition */
        while (1) {
            if (is_prime(candidate)) {
                bool ok = false;
                if (candidate > 10 && is_prime(candidate - 10))
                    ok = true;
                if (is_prime(candidate + 10))
                    ok = true;
                if (ok) {
                    /* Allocate or extend storage if necessary */
                    if (cap == 0) {
                        cap = 16;
                        list = (seq_t *)malloc(cap * sizeof(seq_t));
                        if (!list) exit(EXIT_FAILURE);
                    } else if ((size_t)count >= cap) {
                        cap *= 2;
                        seq_t *tmp = (seq_t *)realloc(list, cap * sizeof(seq_t));
                        if (!tmp) exit(EXIT_FAILURE);
                        list = tmp;
                    }
                    list[count++] = (seq_t)candidate;
                    ++candidate;     /* move past this prime for next search */
                    break;
                }
            }
            ++candidate;             /* test the next integer */
        }
    }

    return list[n - 1];
}
