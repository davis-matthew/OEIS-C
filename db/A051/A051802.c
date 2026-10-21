/*
 * A051802  Nonzero multiplicative digital root of n.
 * https://oeis.org/A051802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base, nice
 * Formula:   not stated
 * Range:     u8 n <= 283, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (iteration): a(n) = multiplicative product of non‑zero digits
   repeatedly applied until the value stabilises.  a(0) = 1.  */

/* use the provided seq_t and idx_t types */
seq_t a(idx_t n)
{
    if (n == 0)          /* special case for n = 0 */
        return (seq_t)1;

    seq_t x = (seq_t)n;   /* current value to process */

    for (;;)
    {
        seq_t prod = (seq_t)1;    /* product of non‑zero digits */

        seq_t tmp = x;
        while (tmp > 0)
        {
            seq_t d = tmp % (seq_t)10;
            if (d != 0)
                prod *= d;
            tmp /= (seq_t)10;
        }

        if (prod == x)          /* stabilised -> final root */
            return x;

        x = prod;                /* continue with new product */
    }
}
