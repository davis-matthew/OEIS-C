/*
 * A186499  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f(i)=i^2 and g(j)=-4+5j^2. Complement
 *          of A186500.
 * https://oeis.org/A186499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Complement of A186500: a(n) = n-th positive integer not in the
   sequence b(k) = k + floor(sqrt(5*k*k - 4)).  The algorithm
   iterates through the integers, skipping those equal to b(k), and
   returning the nth remaining value. */

#include "../utils/floor_sqrt.h"

static inline seq_t b_of(idx_t k)
{
    /* b(k) = k + floor(sqrt(5*k*k - 4)) */
    seq_t val = (seq_t)5 * k * k - 4;
    seq_t sq = floor_sqrt(val);
    return k + sq;
}

seq_t a(idx_t n)
{
    idx_t   k      = 1;          // index in b sequence
    seq_t   next_b = b_of(k);    // current value of b(k)
    seq_t   cur    = 1;          // candidate integer to test
    idx_t   missing = 0;         // count of integers not in b

    while (1) {
        if (cur == next_b) {          // cur is in b(k), skip it
            k++;
            next_b = b_of(k);
            cur++;
        } else {                      // cur is a missing integer
            missing++;
            if (missing == n)
                return cur;
            cur++;
        }
    }
}
