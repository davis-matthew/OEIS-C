/*
 * A097736  Pell equation solutions (8*b(n))^2 - 65*a(n)^2 = -1 with b(n) =
 *          A097735(n), n >= 0.
 * https://oeis.org/A097736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A097736 – Pell equation solutions (8*b(n))^2 - 65*a(n)^2 = -1
   Recurrence: a(0)=1, a(1)=257, a(n)=258*a(n-1)-a(n-2)   (n>=2) */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)257;
    }

    /* Iterate the linear recurrence.
       The constant 258 is chosen so that 258*a(n-1) > a(n-2),
       thus the unsigned subtraction is always safe. */
    seq_t prev2 = (seq_t)1;       /* a(n-2)   */
    seq_t prev1 = (seq_t)257;     /* a(n-1)   */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = (seq_t)258 * prev1 - prev2;
        prev2  = prev1;
        prev1  = cur;
    }

    return prev1;
}
