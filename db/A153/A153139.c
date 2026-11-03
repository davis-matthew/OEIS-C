/*
 * A153139  Numbers n with property that n^2 is a concatenation of three
 *          3-digit primes.
 * https://oeis.org/A153139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 59 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Compute the OEIS sequence A153139.
   The sequence consists of all integers n (10000 ≤ n ≤ 31622) such that
   n^2 is a 9‑digit decimal number whose three 3‑digit groups are
   prime numbers.  There are exactly 60 such n.
*/

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* static helper to check primality of numbers up to 999 */
static bool is_prime_3digit(int x)
{
    if (x < 100) return false;
    if (x % 2 == 0) return x == 2;
    if (x % 3 == 0) return x == 3;
    for (int d = 5; d * d <= x; d += 6) {
        if (x % d == 0 || x % (d + 2) == 0) return false;
    }
    return true;
}

/* build the list of all n satisfying the property, once */
static void build_sequence(seq_t **out, size_t *cnt)
{
    size_t capacity = 64;          /* enough for 60 elements */
    seq_t *arr = mem_new(seq_t, capacity);
    size_t length = 0;

    for (uint64_t n = 10000; n <= 31622; ++n) {
        uint64_t sq = n * n;
        /* split into 3 digit groups */
        int p1 = (int)(sq / 1000000ULL);             /* highest 3 digits */
        int p2 = (int)((sq / 1000ULL) % 1000ULL);    /* middle 3 digits */
        int p3 = (int)(sq % 1000ULL);                /* lowest 3 digits */

        if (is_prime_3digit(p1) && is_prime_3digit(p2) && is_prime_3digit(p3)) {
            if (length == capacity) {
                capacity *= 2;
                arr = mem_resize(seq_t, arr, capacity);
            }
            arr[length++] = (seq_t)n;
        }
    }
    *out = arr;
    *cnt = length;
}

/* main function requested by the build system */
seq_t a(idx_t n)
{
    /* static storage for the sequence */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static bool ready = false;

    if (!ready) {
        build_sequence(&seq, &seq_len);
        ready = true;
    }

    if (n < 1 || (size_t)n > seq_len)
        return (seq_t)0;  /* undefined for out of bounds */

    return seq[n - 1];
}
