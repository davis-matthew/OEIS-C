/*
 * A203981  Number of (n+1) X 6 0..2 arrays with no 2 X 2 subblock having
 *          equal diagonal elements or equal antidiagonal elements, and new
 *          values 0..2 introduced in row major order.
 * https://oeis.org/A203981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 25*a(n-1) - 45*a(n-2) - 963*a(n-3) + 2025*a(n-4) +
 *            3645*a(n-5) - 6561*a(n-6). Initial values: a(1)=1536,
 *            a(2)=31104, a(3)=629856, a(4)=13071456, a(5)=271918944,
 *            a(6)=5671161216
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial terms a(1)..a(6).  Stored in unsigned type seq_t. */
    static const seq_t init[6] = {
        (seq_t)1536ULL,
        (seq_t)31104ULL,
        (seq_t)629856ULL,
        (seq_t)13071456ULL,
        (seq_t)271918944ULL,
        (seq_t)5671161216ULL
    };

    if (n <= 6)
    {
        /* Direct return for indexes 1..6 */
        return init[n - (idx_t)1];
    }

    /* Working variables hold the last six values:
       a0=a(n-6), a1=a(n-5), a2=a(n-4), a3=a(n-3),
       a4=a(n-2), a5=a(n-1). */
    seq_t a0 = init[0];
    seq_t a1 = init[1];
    seq_t a2 = init[2];
    seq_t a3 = init[3];
    seq_t a4 = init[4];
    seq_t a5 = init[5];
    seq_t an = (seq_t)0;   /* will hold a(k) during loop */

    /* Compute terms from n=7 up to desired n. */
    for (idx_t k = (idx_t)7; k <= n; ++k)
    {
        /* Calculate using only unsigned arithmetic.
           pos = 25*a5 + 2025*a2 + 3645*a1
           neg = 45*a4 + 963*a3 + 6561*a0
           a(n) = pos - neg   (pos >= neg for the sequence) */
        seq_t pos = a5 * ((seq_t)25) + a2 * ((seq_t)2025) + a1 * ((seq_t)3645);
        seq_t neg = a4 * ((seq_t)45) + a3 * ((seq_t)963)  + a0 * ((seq_t)6561);
        an = pos - neg;   /* unsigned subtraction is safe because pos >= neg */

        /* Shift window for next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = an;
    }

    return an;
}
