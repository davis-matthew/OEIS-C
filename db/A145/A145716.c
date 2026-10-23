/*
 * A145716  Numbers Y such that 381*Y^2+127 is a square.
 * https://oeis.org/A145716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(1)=13,  a(2)=26403,  a(n+2)=2030*a(n+1)-a(n) */
seq_t a(idx_t n)
{
    /* Handle the first two terms directly.  Note that n is 1‑based. */
    if (n == 1) return (seq_t)13;
    if (n == 2) return (seq_t)26403;

    /* Iteratively compute up to the requested index. */
    seq_t a_prev2 = (seq_t)13;         /* a(1)   */
    seq_t a_prev1 = (seq_t)26403;      /* a(2)   */
    seq_t next;

    for (idx_t i = 3; i <= n; ++i) {
#if defined(__GNUC__) || defined(__clang__)
        /* If seq_t is 64‑bit or smaller, use a 128‑bit intermediate to
           avoid overflow before the subtraction. */
        if (sizeof(seq_t) <= 8) {
            unsigned __int128 tmp = (unsigned __int128)a_prev1 * (unsigned __int128)2030u;
            next = (seq_t)(tmp - a_prev2);
        } else
#endif
        {
            /* For larger seq_t types, rely on the built‑in arbitrary‑precision
               arithmetic provided by the compiler / library. */
            next = a_prev1 * (seq_t)2030u - a_prev2;
        }

        a_prev2 = a_prev1;
        a_prev1 = next;
    }

    return a_prev1;
}
