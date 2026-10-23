/*
 * A130236  Partial sums of the 'upper' Fibonacci Inverse A130234.
 * https://oeis.org/A130236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * countFib(n-1) - sumFib(n-1) where countFib(m) =
 *            number of Fibonacci numbers f ≤ m (counting 0 once and 1 twice),
 *            and sumFib(m) = sum of those Fibonacci numbers with the same
 *            multiplicity
 * Range:     u8 n <= 34, u16 n <= 3805, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t m = n - 1;          /* upper bound for the Fibonacci values to include */
    seq_t count = 0;          /* multiplicity‑count of Fibonacci numbers ≤ m   */
    seq_t sum   = 0;          /* sum of those Fibonacci numbers              */

    /* Fibonacci generation: f0 = F_0 = 0, f1 = F_1 = 1, then F_{i+1} = F_i + F_{i-1} */
    seq_t f0 = 0, f1 = 1;
    idx_t step = 0;

    while (1) {
        seq_t f;
        if (step == 0)          /* F_0 = 0 */
            f = 0;
        else if (step == 1)     /* F_1 = 1 (first occurrence) */
            f = 1;
        else {
            f = f0 + f1;        /* F_{step} = F_{step-1} + F_{step-2} */
            f0 = f1;
            f1 = f;
        }

        if (f > m)              /* stop once the Fibonacci number exceeds m */
            break;

        count += 1;             /* count this occurrence */
        sum += f;               /* add to the sum          */
        step += 1;
    }

    /* a(n) = n * count - sum  */
    seq_t result = ((seq_t)n * count) - sum;
    return result;
}
