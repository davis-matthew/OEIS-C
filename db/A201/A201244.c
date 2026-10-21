/*
 * A201244  Number of ways to place 3 non-attacking ferses on an n X n board.
 * https://oeis.org/A201244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-2)*(n^5 + 2n^4 - 11n^3 + 2n^2 + 54n - 60)/6, n>=2
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 54, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 0, a(2) = 0 */

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;

    /* Use 128‑bit intermediates for 64‑bit or smaller seq_t to avoid
       overflow in the polynomial evaluation. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 x = (unsigned __int128)n;      /* n as 128‑bit */
        unsigned __int128 n2 = x * x;          /* n^2 */
        unsigned __int128 n3 = n2 * x;         /* n^3 */
        unsigned __int128 n4 = n3 * x;         /* n^4 */
        unsigned __int128 n5 = n4 * x;         /* n^5 */

        unsigned __int128 val = n5
                               + (unsigned __int128)2 * n4
                               - (unsigned __int128)11 * n3
                               + (unsigned __int128)2 * n2
                               + (unsigned __int128)54 * x
                               - (unsigned __int128)60;

        val *= (x - (unsigned __int128)2);
        val /= (unsigned __int128)6;

        return (seq_t)val;   /* final result fits into seq_t */
    }

    /* For 128‑bit or larger seq_t we can use native operations directly. */
    seq_t nn  = (seq_t)n;
    seq_t n2  = nn * nn;
    seq_t n3  = n2 * nn;
    seq_t n4  = n3 * nn;
    seq_t n5  = n4 * nn;

    seq_t val = n5
              + (seq_t)2 * n4
              - (seq_t)11 * n3
              + (seq_t)2 * n2
              + (seq_t)54 * nn
              - (seq_t)60;

    val *= (nn - (seq_t)2);
    val /= (seq_t)6;

    return val;
}
