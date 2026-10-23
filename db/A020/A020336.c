/*
 * A020336  Numbers whose base-8 representation is the juxtaposition of two
 *          identical strings.
 * https://oeis.org/A020336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n * 8^floor(log_8(n)+1) + n
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This yields the base‑8 concatenation of the string representation of n with itself. */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          /* cast n to seq_t once for all arithmetic */
    idx_t  tmp = n;               /* work on a copy for digit counting */
    seq_t  pow = 1;               /* 8^k will be accumulated here */

    /* Count digits of n in base‑8 and simultaneously compute 8^k. */
    while (tmp)
    {
        tmp /= 8;                 /* shift one base‑8 digit away */
        pow *= 8;                 /* multiply by 8 to build 8^k */
    }

    /* Final formula: n*8^k + n */
    return sn * pow + sn;
}
