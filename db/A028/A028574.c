/*
 * A028574  Expansion of 1/((1-16*x)^2*(1 - 14*x + 56*x^2 - 64*x^3)).
 * https://oeis.org/A028574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n*(3 - 49*2^(n+1) + 147*2^(2*n+3) + (21*n
 *            -10)*2^(3*n+6))/441. Recurrence: a(n) = 46*a(n-1) - 760*a(n-2) +
 *            5440*a(n-3) - 16384*a(n-4) + 16384*a(n-5). Initial terms:
 *            a(0)=1, a(1)=46, a(2)=1356, a(3)=32856, a(4)=714672
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Stored as literals; for seq_t types narrower than 32 bits the values are truncated
       modulo 2^width, which matches the correct residue of the true integer value.     */
    static const seq_t init[5] = {
        (seq_t)1,
        (seq_t)46,
        (seq_t)1356,
        (seq_t)32856,
        (seq_t)714672
    };

    if (n < 5)
        return init[n];

    seq_t a0 = init[0];
    seq_t a1 = init[1];
    seq_t a2 = init[2];
    seq_t a3 = init[3];
    seq_t a4 = init[4];
    seq_t an = 0;

    for (idx_t i = 5; i <= n; ++i) {
        /* Compute a(n) = 46*a(n-1) - 760*a(n-2) + 5440*a(n-3) - 16384*a(n-4) + 16384*a(n-5).
           We perform the positive contributions first, then subtract the rest, avoiding
           underflow in unsigned arithmetic.                                              */
        seq_t pos = (seq_t)46 * a4 + (seq_t)5440 * a2 + (seq_t)16384 * a0;
        seq_t neg = (seq_t)760 * a3 + (seq_t)16384 * a1;
        an = pos - neg;

        /* Shift the window of previous terms.                                          */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }

    return an;
}
