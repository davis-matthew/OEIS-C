/*
 * A179477  Antonym of A014824: each term is 10 times the previous term minus
 *          n.
 * https://oeis.org/A179477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 12*a(n-1) - 21*a(n-2) + 10*a(n-3) for n>2
   with initial values a(0)=10, a(1)=99, a(2)=988 */

seq_t a(idx_t n)
{
    if (n < 3) {
        switch (n) {
            case 0: return (seq_t)10;
            case 1: return (seq_t)99;
            case 2: return (seq_t)988;
        }
    }

    seq_t a0 = (seq_t)10;   /* a(n-3) */
    seq_t a1 = (seq_t)99;   /* a(n-2) */
    seq_t a2 = (seq_t)988;  /* a(n-1) */
    seq_t ai;

    for (idx_t i = 3; i <= n; ++i) {
        ai = 12 * a2 - 21 * a1 + 10 * a0;
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
