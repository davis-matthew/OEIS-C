/*
 * A076505  3 people at a party are saying Hello to each other. Person 1 says
 *          Hello. Person 2 counts the times Hello has been said and says
 *          Hello twice that number. Person 3 says Hello 3 times the sum of
 *          Hello's and then it is Person 1 again. This is how many Hello's
 *          each person says.
 * https://oeis.org/A076505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=2, a(3)=9, a(4)=12;
   for n>4, a(n)=24*a(n-3). */
seq_t a(idx_t n)
{
    /* Base values */
    static const seq_t base[5] = {0, (seq_t)1, (seq_t)2, (seq_t)9, (seq_t)12};

    /* Reduce n to the equivalent index in {1,2,3,4} by stepping back
       multiples of 3. Count how many 24‑multiplications are needed. */
    idx_t m = n;
    while (m > 4) {
        m -= 3;
    }

    /* Start from the base value for the reduced index. */
    seq_t result = base[m];

    /* Multiply by 24 for each backward step taken. */
    for (idx_t i = 0; i < (n - m) / 3; ++i) {
        result *= (seq_t)24;
    }

    return result;
}
