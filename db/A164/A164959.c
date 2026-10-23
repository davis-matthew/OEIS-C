/*
 * A164959  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^8 = I.
 * https://oeis.org/A164959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: for n >= 9,
   a(n) = 22·(a(n-1)+a(n-2)+…+a(n-7)) - 253·a(n-8).
   Initial values (OEIS A164959):
     a(0)=1, a(1)=24, a(2)=552, a(3)=12696, a(4)=292008,
     a(5)=6716184, a(6)=154472232, a(7)=3552861336.
   a(8) is obtained from the same recurrence with an extra +1 term:
     a(8) = 22·(a(1)+…+a(7)) - 253·a(0) + 1.  */

seq_t a(idx_t n)
{
    /* Pre‑computed initial values */
    static const seq_t init[8] = {
        (seq_t)1,
        (seq_t)24,
        (seq_t)552,
        (seq_t)12696,
        (seq_t)292008,
        (seq_t)6716184,
        (seq_t)154472232,
        (seq_t)3552861336
    };

    if (n < 8)                     /* return small indices directly */
        return init[n];

    /* ring buffer of the last 8 values */
    seq_t ring[8];
    for (int i = 0; i < 8; ++i)
        ring[i] = init[i];

    /* compute a(8) (special case, contains the +1 term) */
    seq_t sum7 = 0;
    for (int j = 1; j <= 7; ++j)
        sum7 += ring[(8 - j) % 8];
    seq_t a8 = sum7 * (seq_t)22 - (seq_t)253 * ring[(8 - 8) % 8] + 1;
    ring[8 % 8] = a8;
    if (n == 8)
        return a8;

    /* iterate from 9 up to n */
    for (idx_t i = 9; i <= n; ++i) {
        seq_t sum = 0;
        for (int j = 1; j <= 7; ++j)
            sum += ring[(i - j) % 8];
        seq_t term = sum * (seq_t)22 - (seq_t)253 * ring[(i - 8) % 8];
        ring[i % 8] = term;
        if (i == n)
            return term;
    }

    /* unreachable, but keep compiler happy */
    return (seq_t)0;
}
