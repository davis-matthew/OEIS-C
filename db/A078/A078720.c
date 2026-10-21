/*
 * A078720  Integer part of the ratio of even to odd terms among n, f(n),
 *          f(f(n)), ...., 1 for the Collatz function (that is, until reaching
 *          "1" for the first time), or -1 if 1 is never reached.
 * https://oeis.org/A078720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = floor(even_count/odd_count) where even_count and
 *            odd_count are * the numbers of even and odd terms in the Collatz
 *            sequence starting at n * and proceeding until the first
 *            occurrence of 1
 * Range:     u8 n <= 58, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns -1 if 1 is never reached (theoretically not needed
 * for standard inputs). */
seq_t a(idx_t n)
{
    /* Convert the index to the sequence type for internal use */
    seq_t curr = (seq_t)n;
    seq_t even = (seq_t)0;
    seq_t odd  = (seq_t)0;

    /* Process terms until 1 is reached */
    for (;;)
    {
        /* Count parity of the current term */
        if ((curr & (seq_t)1) == (seq_t)0)
            even++;
        else
            odd++;

        /* Stop when 1 is encountered */
        if (curr == (seq_t)1)
            break;

        /* Apply the Collatz transformation */
        if ((curr & (seq_t)1) == (seq_t)0)
            curr = curr / (seq_t)2;
        else
            curr = (seq_t)3 * curr + (seq_t)1;
    }

    /* If odd = 0 (theoretical) return -1 */
    if (odd == (seq_t)0)
        return (seq_t)-1;

    /* Integer division automatically floors the result */
    return even / odd;
}
