/*
 * A018527  Divisors of 575.
 * https://oeis.org/A018527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 575: a(n) = n-th smallest divisor of 575. */
/* (A018527) */

/* This function enumerates the divisors of 575 (5²·23) on its first call,
   sorts them, and then returns the nth divisor (1 ≤ n ≤ 6).  For n > 6
   it returns 0, as the sequence is finite.  The generation is independent
   of any external helper routines.  */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static storage for the sorted divisors, computed once */
    static seq_t divisors[6];
    static bool initialized = false;

    if (!initialized) {
        /* generate all 6 divisors of 575 = 5^2 * 23 */
        size_t count = 0;
        for (uint64_t a = 0; a <= 2; ++a) {         /* exponent of 5 */
            uint64_t pow5 = 1;
            for (uint64_t i = 0; i < a; ++i) pow5 *= 5;
            for (uint64_t b = 0; b <= 1; ++b) {     /* exponent of 23 */
                uint64_t pow23 = 1;
                if (b == 1) pow23 = 23;
                divisors[count++] = (seq_t)(pow5 * pow23);
            }
        }

        /* simple bubble sort for the six values             */
        for (size_t i = 0; i < 5; ++i)
            for (size_t j = i + 1; j < 6; ++j)
                if (divisors[j] < divisors[i]) {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }
        initialized = true;
    }

    if (n >= 1 && n <= 6)
        return divisors[n - 1];
    return (seq_t)0;
}
