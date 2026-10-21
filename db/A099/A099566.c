/*
 * A099566  A099565(n)/n.
 * https://oeis.org/A099566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of Fibonacci numbers F(2) … F(m+1) where m is the
 *            smallest integer such that Σ_{k=1}^{m} F(k) ≥ n
 * Range:     u8 n <= 12, u16 n <= 33, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* out of bounds; defined to return 0 */

    /* Fibonacci numbers: F(1)=1, F(2)=1, F(3)=2, … */
    seq_t f1 = 1; /* F(1) */
    seq_t f2 = 1; /* F(2) */

    /* product of Fibonacci numbers for the current run (run 1) */
    seq_t product = f2;   /* F(2) = 1 */
    /* cumulative sum of run lengths (Σ_{k=1}^{m} F(k)) */
    seq_t sum = f1;       /* F(1) = 1 */

    if (n <= sum)
        return product;   /* n in the first run */

    /* iterate over runs m = 2, 3, … */
    while (1) {
        seq_t f_next = f1 + f2; /* F(m+1) */
        product *= f_next;      /* include this Fibonacci in the product */
        sum += f2;              /* add the run length F(m) */

        if (n <= sum)
            return product;     /* n lies in this run */

        /* advance Fibonacci indices for next iteration */
        f1 = f2;
        f2 = f_next;
    }
}
