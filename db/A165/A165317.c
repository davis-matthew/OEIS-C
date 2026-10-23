/*
 * A165317  a(n) = the number of digits in the binary representation of n that
 *          each do not precede or follow a similarly valued digit.
 * https://oeis.org/A165317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = number of binary digits of n that are isolated, * i.e.
 *            the number of runs of length 1 in the binary expansion of n. *
 *            (Runs of a single 0 or a single 1.)
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Count isolated bits in the binary representation of n. */
seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence offset. */
    idx_t tmp = n;
    /* Determine number of bits in n. */
    size_t bitlen = 0;
    while (tmp)
    {
        bitlen++;
        tmp >>= 1;
    }

    seq_t count   = 0;
    seq_t run_len = 0;
    int   last_bit = -1;   /* -1 means “undefined” before first bit */

    /* Process bits from most–significant to least–significant. */
    for (size_t i = bitlen; i > 0; --i)
    {
        int bit = (int)((n >> (i - 1)) & 1ULL);

        if (bit == last_bit)
        {
            /* Continues the current run. */
            run_len++;
        }
        else
        {
            /* A new run starts.  The previous run ends. */
            if (run_len == 1)
                count++;          /* isolated run counted */
            run_len   = 1;
            last_bit  = bit;
        }
    }

    /* Handle the final run. */
    if (run_len == 1)
        count++;

    return count;
}
