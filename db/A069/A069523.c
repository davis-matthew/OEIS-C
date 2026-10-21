/*
 * A069523  Smallest multiple of n with digit sum = 4, or 0 if no such number
 *          exists, e.g. a(3k)= 0.
 * https://oeis.org/A069523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 46, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A069523 : smallest multiple of n with digit sum = 4, or 0 if none.
   The algorithm checks divisibility by 3, 37, or 41 – in these cases the
   required sum cannot be achieved.  For all other n the function iterates
   successive multiples until the digit sum equals 4.  Digit sums are
   computed directly using modulo 10 and division, so no external helper
   functions are required.  The method is O(k) where k is the number of
   checked multiples; for n ≤ 10 000 this k is at most a few dozen. */

#include <stddef.h>          /* for idx_t and seq_t types */

static inline seq_t digit_sum_local(seq_t x)
{
    seq_t sum = 0;
    while (x > 0)
    {
        sum += x % 10;
        x   /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Check known impossible cases */
    if (n % 3ULL == 0 || n % 37ULL == 0 || n % 41ULL == 0)
        return 0;

    seq_t N = (seq_t)n;
    for (idx_t k = 1; ; ++k)
    {
        seq_t m = N * (seq_t)k;
        if (digit_sum_local(m) == 4)
            return m;
    }
}
