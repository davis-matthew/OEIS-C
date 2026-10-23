/*
 * A138105  Partial sums of non-Fibonacci numbers A001690.
 * https://oeis.org/A138105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{j=1..n} A001690(j)
 * Range:     u8 n <= 17, u16 n <= 351, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the sum of the first n non‑Fibonacci numbers. */
seq_t a(idx_t n)
{
    /* We generate the non‑Fibonacci numbers in ascending order, keeping a
       running sum.  Fibonacci numbers are generated on the fly with a
       simple recurrence.  The loop stops after all n non‑Fibonacci numbers
       are collected. */
    idx_t i = 1;          /* current integer being examined   */
    idx_t f1 = 1, f2 = 1; /* the two last Fibonacci numbers   */
    seq_t sum = 0;        /* running sum of non‑Fibonacci numbers */
    idx_t count = 0;      /* how many non‑Fibonacci numbers seen */

    while (count < n) {
        /* Advance Fibonacci pointer until f2 >= i */
        while (i > f2) {
            idx_t temp = f2;
            f2 = f1 + f2;
            f1 = temp;
        }

        /* If i is a Fibonacci number (either f1 or f2), skip it. */
        if (i == f1 || i == f2) {
            /* do nothing, just move on to next i */
        } else {
            sum += (seq_t)i;
            ++count;
        }
        ++i;
    }
    return sum;
}
