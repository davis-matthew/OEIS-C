/*
 * A177869  Integers divisible by their number of digits in binary.
 * https://oeis.org/A177869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Integers divisible by their number of binary digits. */
/* a(n) is the nth positive integer k such that k % bitlen(k) == 0, */
/* where bitlen(k) = floor(log2(k)) + 1. */
/* Example: a(1)=1, a(2)=2, a(3)=6, a(4)=8, … */

seq_t a(idx_t n)
{
    idx_t count = 0;     /* how many terms found so far          */
    seq_t k = 0;         /* candidate integer                    */

    while (count < n)
    {
        ++k;             /* next integer to test                 */

        /* compute bit length of k */
        int bits = 0;
        seq_t tmp = k;
        while (tmp)
        {
            tmp >>= 1;
            ++bits;
        }

        /* test divisibility by number of bits */
        if ((k % (seq_t)bits) == 0)
        {
            ++count;      /* found a term */
        }
    }

    return k;            /* the nth term */
}
