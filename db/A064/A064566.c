/*
 * A064566  Reciprocal of n terminates with an infinite repetition of digit 7.
 *          Multiples of 10 are omitted.
 * https://oeis.org/A064566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A064566: numbers of the form 9*2^(6i-4) and 9*5^(6i-2) (i>0),
   sorted in increasing order. */

seq_t a(idx_t n)
{
    /* Initial terms of the two geometric progressions */
    seq_t next1 = 36;           /* 9*2^(2)      */
    seq_t next2 = 5625;         /* 9*5^(4)      */
    /* Multipliers between consecutive terms in each progression */
    const seq_t mul1 = 64;      /* 2^6   */
    const seq_t mul2 = 15625;   /* 5^6   */

    seq_t result = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        if (next1 <= next2) {
            result = next1;
            next1 = next1 * mul1;
        } else {
            result = next2;
            next2 = next2 * mul2;
        }
    }
    return result;
}
