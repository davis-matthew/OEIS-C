/*
 * A087665  Consider recurrence b(0) = n/4, b(k) = b(k-1)*floor(b(k-1));
 *          sequence gives first integer reached, or -1 if no integer is ever
 *          reached.
 * https://oeis.org/A087665
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 12, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  b(0) = n/4,  b(k) = b(k-1)*⌊b(k-1)⌋.
   a(n) is the first integer value of b(k); returns -1 if no integer is ever
   reached.  Indices start at n=8, but the function works for any n. */
seq_t a(idx_t n)
{
    seq_t num = (seq_t)n;           /* numerator of b(k) = num / 4          */
    /* iterate until the fraction becomes an integer                      */
    /* (i.e. num % 4 == 0).  According to the conjecture this always
       happens for n ≥ 2, but we guard against infinite loops.             */
    unsigned int limit = 20000;      /* safe upper bound for iterations      */
    while ((num % (seq_t)4) != 0)
    {
        if (--limit == 0)            /* no integer found – return -1        */
            return (seq_t)-1;
        seq_t s = num / (seq_t)4;    /* floor(b(k-1))                        */
        num = num * s;               /* b(k) = num / 4 * s                  */
    }
    return num / (seq_t)4;           /* integer value reached                */
}
