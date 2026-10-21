/*
 * A218019  Integers in which the number of distinct base-10 digits is 7.
 * https://oeis.org/A218019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A218019: the nth integer that has exactly seven distinct decimal digits. */
/* The sequence starts at a(1) = 1023456.  Behaviour for n < 1 is undefined. */

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; we just return 0 for invalid n */
        return (seq_t)0;

    idx_t found = 0;          /* number of qualifying integers found so far */
    seq_t candidate = 1023456;/* smallest integer with 7 distinct digits */

    while (1) {
        /* Count distinct decimal digits of candidate */
        unsigned bitmask = 0;   /* bit i set if digit i appears */
        seq_t x = candidate;

        while (x > 0) {
            unsigned d = (unsigned)(x % 10);  /* extract last digit */
            bitmask |= 1u << d;               /* mark that digit */
            x /= 10;                          /* remove last digit */
        }

        /* Count how many bits are set in bitmask */
        unsigned distinct = 0;
        unsigned m = bitmask;
        while (m) {
            distinct += m & 1u;
            m >>= 1;
        }

        if (distinct == 7) {   /* candidate is a term of the sequence */
            ++found;
            if (found == n)
                return candidate;
        }

        ++candidate;          /* check next integer */
    }
}
