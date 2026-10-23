/*
 * A115776  Integers i such that 13*i = A048720bi(21,i), but 15*i <>
 *          A048720bi(23,i).
 * https://oeis.org/A115776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function: a(n) returns the nth integer i such that
 *   13*i == A048720bi(21,i)   (carryless product of 21 and i)
 *   15*i != A048720bi(23,i)   (carryless product of 23 and i)
 *
 * The function scans consecutive integers i starting from 1
 * until it finds the n-th one that satisfies both conditions.
 * The integer i is converted to seq_t before use.
 */

seq_t a(idx_t n)
{
    idx_t i      = 0;          /* current integer to test (uint64_t) */
    idx_t found  = 0;          /* number of solutions found so far */

    while (found < n)
    {
        i++;                                   /* next candidate */

        seq_t I = (seq_t)i;                     /* convert to seq_t */

        /* carryless product of 21 and i: (I << 4) ^ (I << 2) ^ I */
        seq_t carry21 = (I << 4) ^ (I << 2) ^ I;
        /* normal product 13*i */
        seq_t prod13  = I * 13;
        if (carry21 != prod13)
            continue;                          /* fails first condition */

        /* carryless product of 23 and i: (I << 4) ^ (I << 2) ^ (I << 1) ^ I */
        seq_t carry23 = (I << 4) ^ (I << 2) ^ (I << 1) ^ I;
        /* normal product 15*i */
        seq_t prod15  = I * 15;
        if (carry23 == prod15)
            continue;                          /* fails second condition */

        /* satisfies both */
        found++;
    }

    return (seq_t)i;                     /* i is the requested a(n) */
}
