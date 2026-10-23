/*
 * A153083  Numbers such that 2*n + 11 is not prime.
 * https://oeis.org/A153083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) returns the nth positive integer m (n ≥ 1) such that 2*m + 11 is not prime.
 * Implementation uses a simple trial‑division primality test (`is_prime_seq`) based on
 * the helper `floor_sqrt` from the utils library.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"      // provides seq_t floor_sqrt(seq_t x)

static bool is_prime_seq(seq_t n)
{
    /* handle small cases */
    if (n < (seq_t)2) return false;
    if (n == (seq_t)2) return true;
    if ((n & (seq_t)1) == (seq_t)0) return false;   /* even number */

    seq_t limit = floor_sqrt(n);
    for (seq_t d = (seq_t)3; d <= limit; d += (seq_t)2) {
        if (n % d == (seq_t)0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t count = 0;          /* number of terms found so far */
    seq_t candidate = (seq_t)1;  /* current integer to test */

    for (;;) {
        candidate++;
        seq_t val = candidate * (seq_t)2 + (seq_t)11;  /* 2*candidate + 11 */

        if (!is_prime_seq(val)) {
            count++;
            if (count == n) return candidate;
        }
    }
}
