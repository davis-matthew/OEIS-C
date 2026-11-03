/*
 * A095050  Numbers such that all ten digits are needed to write all positive
 *          divisors in decimal representation.
 * https://oeis.org/A095050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 9830 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A095050:
   Numbers n such that all ten decimal digits appear in the decimal
   representations of all positive divisors of n.  The code searches
   successive integers, tests the property, and memoises the found
   numbers.  After the first term (108) all further multiples of
   previously found terms are automatically accepted. */
  
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

/* Return true iff every decimal digit 0–9 occurs in the representation
   of at least one divisor of n.  n is assumed to be >0. */
static bool has_all_digits(uint64_t n)
{
    unsigned char dig[10] = {0};
    for (uint64_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            uint64_t v = d;
            while (v > 0) {
                dig[v % 10] = 1;
                v /= 10;
            }
            uint64_t other = n / d;
            if (other != d) {
                v = other;
                while (v > 0) {
                    dig[v % 10] = 1;
                    v /= 10;
                }
            }
        }
    }
    for (int i = 0; i < 10; ++i)
        if (!dig[i]) return false;
    return true;
}

seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static seq_t *seq = NULL;
    static size_t count = 0;        /* number of terms stored */
    static size_t capacity = 0;     /* size of allocated array */
    static uint64_t tested = 0;     /* last integer examined */

    if (n == 0) {
        /* Undefined behaviour for indices below the offset;
           return zero or any value – it will not be used.      */
        return (seq_t)0;
    }

    /* Initialise storage on first call */
    if (capacity == 0) {
        capacity = 16;
        seq = alloc_seq_array(capacity);
    }

    /* Generate terms until we have at least n of them */
    while (count < n) {
        ++tested;
        if (has_all_digits(tested)) {
            if (count == capacity) {          /* grow array */
                size_t new_cap = capacity << 1;
                seq_t *tmp = alloc_seq_array(new_cap);
                mem_copy(tmp, seq, count * sizeof(seq_t));
                free_seq_array(seq);
                seq = tmp;
                capacity = new_cap;
            }
            seq[count++] = (seq_t)tested;
        }
    }

    return seq[n - 1];
}
