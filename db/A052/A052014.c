/*
 * A052014  Primes with distinct digits in descending order.
 * https://oeis.org/A052014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A052014 – primes whose decimal digits are in strictly
   decreasing order.  The function enumerates all numbers whose
   decimal representation is a strictly decreasing sequence of distinct
   digits, tests each for primality with is_prime_small, stores the
   primes found, sorts them ascending, and finally returns the n‑th
   prime (n ≥ 1).  Because the total number of such primes is 87,
   a small fixed static array is ample. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"

/* The sequence function requested by the build system. */
seq_t a(idx_t n)
{
    /* 87 primes – a few extra slots for safety. */
    static seq_t primes[128];
    static size_t primes_count = 0;
    static bool initialized = false;

    if (!initialized) {
        size_t count = 0;
        /* Enumerate every non‑empty subset of the digits {0…9}.  For
           each subset, arrange the digits in descending order to form
           a number with strictly decreasing digits (leading zeroes
           produce a smaller number and are therefore excluded). */
        for (uint16_t mask = 1; mask < (1U << 10); ++mask) {
            seq_t num = 0;
            for (int d = 9; d >= 0; --d) {
                if (mask & (1U << d))
                    num = num * 10 + (seq_t)d;
            }
            if (num != 0 &&
                is_prime_small((uint64_t)num))
                primes[count++] = num;
        }

        /* Insertion sort to order the primes ascending. */
        for (size_t i = 1; i < count; ++i) {
            seq_t key = primes[i];
            size_t j = i;
            while (j > 0 && primes[j - 1] > key) {
                primes[j] = primes[j - 1];
                --j;
            }
            primes[j] = key;
        }

        primes_count = count;
        initialized = true;
    }

    if (n < 1 || (size_t)n > primes_count)
        return (seq_t)0;          /* undefined for out‑of‑range n */

    return primes[n - 1];
}
