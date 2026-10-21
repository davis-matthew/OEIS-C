/*
 * A078412  a(0) = 5, a(1) = 8; for n > 1, a(n)=(a(n-1)+a(n-2))/3^k, where 3^k
 *          is the highest power of 3 dividing a(n-1)+a(n-2).
 * https://oeis.org/A078412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursive definition: a(0) = 5, a(1) = 8; for n > 1,
   a(n) = (a(n-1) + a(n-2)) / 3^k, where 3^k is the largest power
   of 3 dividing the sum a(n-1) + a(n-2). */
seq_t a(idx_t n)
{
    /* Handle the initial terms explicitly. */
    if (n == 0) return (seq_t)5;
    if (n == 1) return (seq_t)8;

    /* Iteratively compute from a(0) up to a(n). */
    seq_t prev2 = (seq_t)5;   /* a(n-2) */
    seq_t prev1 = (seq_t)8;   /* a(n-1) */
    seq_t curr = 0;

    for (idx_t i = 2; i <= n; ++i)
    {
        curr = prev1 + prev2;              /* a(n-1)+a(n-2) */
        /* Divide out all factors of 3. */
        while ((curr % 3) == 0) 
            curr /= 3;
        /* Shift the window for the next iteration. */
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
