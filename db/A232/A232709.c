/*
 * A232709  Nonnegative integers such that the sum of digits mod 10 equals the
 *          product of digits mod 10.
 * https://oeis.org/A232709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 6817, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A232709: Nonnegative integers such that
 *   (sum of digits) mod 10 == (product of digits) mod 10.
 * The function returns the n-th term (1‑based indexing). */
seq_t a(idx_t n)
{
    seq_t val   = 0;          /* candidate integer */
    idx_t count = 0;          /* how many valid numbers found so far */

    while (count < n)
    {
        seq_t x   = val;
        seq_t s   = 0;        /* sum of digits modulo 10 */
        seq_t p   = 1;        /* product of digits modulo 10 */

        if (x == 0)          /* special handling for 0 */
        {
            s = 0;
            p = 0;
        }
        else
        {
            while (x > 0)
            {
                seq_t d = x % 10;          /* current digit */
                s = (s + d) % 10;          /* accumulate sum mod 10 */
                p = (p * d) % 10;          /* accumulate product mod 10 */
                x /= 10;
            }
        }

        if ((s % 10) == (p % 10))
        {
            count++;
            if (count == n)
                return val;                 /* found the n-th term */
        }

        val++;                               /* test next integer */
    }

    /* The loop guarantees a return, but silence the compiler warning. */
    return 0;
}
