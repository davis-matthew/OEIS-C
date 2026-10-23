/*
 * A046790  Positive numbers divisible by 8 or by the square of an odd prime.
 * https://oeis.org/A046790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth positive integer divisible by 8
   or by the square of an odd prime.  */
#include <stdbool.h>

/* Return true if m is divisible by 8 or by p² for some odd prime p. */
static bool is_good(seq_t m)
{
    /* Divisible by 8? */
    if ((m & 7) == 0)
        return true;

    /* Check for an odd prime square divisor. */
    for (uint64_t p = 3; ; p += 2) {
        seq_t sq = (seq_t)p * (seq_t)p;
        if (sq > m)
            break;                 /* No larger square can divide m. */
        if ((m % (seq_t)p) != 0)
            continue;              /* p is not a factor of m. */

        /* Count multiplicity of this prime factor. */
        int exp = 0;
        seq_t t = m;
        while ((t % (seq_t)p) == 0) {
            t /= (seq_t)p;
            if (++exp >= 2)
                return true;       /* Found a square of an odd prime. */
        }
    }
    return false;
}

/* Return the nth term of OEIS A046790. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* Undefined for n = 0; safe guard. */

    idx_t count = 0;
    seq_t m = 1;
    for (;;) {
        if (is_good(m)) {
            ++count;
            if (count == n)
                return m;
        }
        ++m;
    }
}
