/*
 * A214300  Sum of the terms of the Pisano period mod n.
 * https://oeis.org/A214300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0}^{T(n)-1} (F_k mod n)
 * Range:     u8 n <= 9, u16 n <= 224, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where T(n) is the Pisano period of the Fibonacci sequence modulo n. */
/* The function iterates the Pisano period and accumulates the terms. */

seq_t a(idx_t n)
{
    /* The sequence is undefined below offset 1; n ≥ 1. */
    if (n == 1)
        return 0;                /* a(1) = 0 by definition. */

    seq_t nn = (seq_t)n;         /* Work in seq_t arithmetic. */
    seq_t sum = 0;
    seq_t f0 = 0;                /* F_0 mod n */
    seq_t f1 = 1 % nn;           /* F_1 mod n (equal to 1 for n>1) */

    /* Do-while loop generates the Pisano period. */
    do
    {
        sum += f0;                /* Add current term. */
        seq_t next = f0 + f1;     /* Next Fibonacci number. */
        if (next >= nn)
            next -= nn;           /* Perform modulo n without overflow for 32/64-bit seq_t. */
        f0 = f1;
        f1 = next;
    } while (!(f0 == 0 && f1 == 1)); /* Stop when the original state repeats. */

    return sum;
}
