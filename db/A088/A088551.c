/*
 * A088551  Fibonacci winding number: the number of 'mod n' operations in one
 *          cycle of the Fibonacci sequence modulo n.
 * https://oeis.org/A088551
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = number of ‘mod n’ operations performed while generating
 *            the Fibonacci sequence modulo n until the first return to the
 *            seed pair (0,1). That is, the count of additions where the sum
 *            of two consecutive residues is ≥ n
 * Range:     u8 n <= 149, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 2 by definition of the sequence.  For n < 2 the value is
       irrelevant, but we return 0 in that case. */
    if (n < 2)
        return 0;

    seq_t prev = 0;     /* F(0) mod n */
    seq_t curr = 1;     /* F(1) mod n */
    idx_t modcount = 0;

    for (;;)
    {
        /* Compute the next Fibonacci residue.  The sum may exceed the
           range of seq_t, so perform it in idx_t before the modulo. */
        idx_t sum = (idx_t)prev + (idx_t)curr;

        /* Count a mod operation exactly when the sum is at least n. */
        if (sum >= n)
            ++modcount;

        seq_t next = (seq_t)(sum % n);

        /* Advance the pair. */
        prev = curr;
        curr = next;

        /* The cycle is complete when the pair returns to (0,1).  The
           initial pair (0,1) is not counted as a cycle completion. */
        if (prev == 0 && curr == 1)
            break;
    }

    return (seq_t)modcount;
}
