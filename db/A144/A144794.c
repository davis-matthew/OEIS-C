/*
 * A144794  A positive integer n is included if every 0 in binary n is next to
 *          at least one other 0.
 * https://oeis.org/A144794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 7722, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence enumerates the positive integers whose binary representation
 * contains no isolated zeros.  That is, every 0 is adjacent to at least
 * one other 0.  a(n) is the nth such integer for n ≥ 1.  This is
 * implemented by a linear scan that tests each candidate integer for
 * the “no isolated zero” property.
 */

#include <stdbool.h>

/* Return the number of bits required to represent x (x > 0). */
static inline int bitlen_seq_t(seq_t x)
{
    int len = 0;
    while (x) {
        x >>= 1;
        len++;
    }
    return len;
}

/* Test whether a given value satisfies the “no isolated zero” property.
 * The function first checks that the value is not all ones (which would
 * have no zeros) and then verifies that every zero has at least one
 * adjacent zero.  The check uses an array of bits; for the range of
 * values considered (the first 10 000 terms) a 128‑bit array is more
 * than sufficient. */
static bool is_valid(seq_t x)
{
    if (x == 0)
        return false;           /* 0 is not in the sequence */

    /* Fast check for all‑ones: count zeros. */
    seq_t tmp = x;
    bool has_zero = false;
    while (tmp) {
        if ((tmp & 1) == 0)
            has_zero = true;
        tmp >>= 1;
    }
    if (!has_zero)
        return false;           /* all bits are 1 */

    /* Determine bit length. */
    int len = bitlen_seq_t(x);
    if (len > 128) {
        /* The 10 000th term is well below 2^128; this case is
         * theoretically impossible for the required range. */
        return false;
    }

    /* Extract bits into a small static array. */
    unsigned char bits[128];
    tmp = x;
    int pos = len - 1;
    while (tmp) {
        bits[pos--] = (unsigned char)(tmp & 1);
        tmp >>= 1;
    }

    /* Verify the condition for each zero. */
    for (int i = 0; i < len; ++i) {
        if (bits[i] == 0) {
            bool left  = (i > 0)   && (bits[i - 1] == 0);
            bool right = (i + 1 < len) && (bits[i + 1] == 0);
            if (!left && !right)
                return false;   /* isolated zero detected */
        }
    }
    return true;
}

/* Return the nth element of the sequence (n ≥ 1).  The function
 * performs a sequential search that increments a candidate number
 * and counts the numbers that satisfy the property until the desired
 * index is reached.  This is O(n·bitlen) and easily fast enough for
 * n up to 10 000. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for n = 0, harmless */

    idx_t found = 0;
    seq_t candidate = 1;

    while (found < n) {
        if (is_valid(candidate))
            ++found;
        if (found == n)
            return candidate;
        ++candidate;
    }

    return candidate;           /* should never be reached */
}
