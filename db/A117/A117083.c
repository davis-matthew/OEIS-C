/*
 * A117083  Octagonal numbers for which the product of the digits is also an
 *          octagonal number.
 * https://oeis.org/A117083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th octagonal number such that the product of its
 *            decimal digits is also an octagonal number. The sequence is
 *            indexed from 0, so a(0) = 0. The function compiles with `seq_t`
 *            being an unsigned integer type (uint8_t ... unsigned __int128)
 *            or an arbitrary‑precision integer class. It uses...
 * Range:     u8 n <= 3, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Integer square root: returns floor(sqrt(x)) for any non‑negative seq_t.
   Uses only integer operations and works for any seq_t type that supports
   + - * / and the remainder operators. */
static inline seq_t isqrt_seq(seq_t x)
{
    seq_t low  = (seq_t)0;
    seq_t high = x;
    while (low < high) {
        seq_t mid = low + ((high - low + (seq_t)1) >> 1);
        /* mid <= high, so mid * mid may overflow; compare using division. */
        if (mid == (seq_t)0 || mid <= x / mid)
            low = mid;
        else
            high = mid - (seq_t)1;
    }
    return low;
}

/* Product of the decimal digits of x.  For x == 0 the product is defined to
   be 0.  */
static seq_t digit_product(seq_t x)
{
    if (x == (seq_t)0)
        return (seq_t)0;
    seq_t prod = (seq_t)1;
    while (x != (seq_t)0) {
        seq_t dig = x % (seq_t)10;
        prod *= dig;
        x /= (seq_t)10;
    }
    return prod;
}

/* Test whether a given non‑negative seq_t m is an octagonal number.
   Octagonal numbers satisfy m = n*(3*n - 2) for some integer n >= 0.
   The method solves the quadratic equation 3n^2 - 2n - m = 0. */
static bool is_octagonal(seq_t m)
{
    if (m == (seq_t)0)
        return true;

    /* D = 4 + 12*m must be a perfect square. */
    seq_t D = (seq_t)4 + (seq_t)12 * m;
    seq_t sqrtD = isqrt_seq(D);

    if (sqrtD * sqrtD != D)
        return false;

    /* (2 + sqrtD) must be divisible by 6. */
    return ((seq_t)2 + sqrtD) % (seq_t)6 == (seq_t)0;
}

/* Main entry function for the sequence.  It is safe for repeated calls
   with arbitrary ordering of n.  The algorithm simply enumerates all
   octagonal numbers and counts those whose digit product is also
   octagonal. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;      /* the initial 0 term */

    idx_t count = (idx_t)0;   /* how many qualifying octagonal numbers found */
    idx_t k = (idx_t)1;       /* octagonal index (n in O(n)=n*(3n-2)) */

    while (1) {
        seq_t kseq = (seq_t)k;
        seq_t oct  = kseq * (((seq_t)3 * kseq) - (seq_t)2);

        seq_t prod = digit_product(oct);
        if (is_octagonal(prod)) {
            ++count;
            if (count == n)
                return oct;
        }
        ++k;
    }
}
