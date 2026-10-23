/*
 * A166520  a(n) = (10*n + 11*(-1)^n + 5)/4.
 * https://oeis.org/A166520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 11*(-1)^n + 5) / 4
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1, a(2) = 9, a(3) = 6, ... (OEIS offset 1) */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. Behaviour for n == 0 is irrelevant. */
    seq_t nn = (seq_t)n;               /* index to seq_t */
    seq_t val = (seq_t)10 * nn + (seq_t)5;  /* 10*n + 5 */

    /* Add 11 for even n, subtract 11 for odd n to account for 11*(-1)^n */
    if ((n & 1) == 0)                   /* even n */
        val += (seq_t)11;
    else                                /* odd n */
        val -= (seq_t)11;

    return val / (seq_t)4;
}
