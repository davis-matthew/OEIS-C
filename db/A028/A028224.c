/*
 * A028224  Expansion of 1/((1-7x)(1-8x)(1-11x)(1-12x)).
 * https://oeis.org/A028224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A028224
   Definition: 1 / ((1-7x)(1-8x)(1-11x)(1-12x))
   Recurrence: a(0)=1, a(1)=38, a(2)=911, a(3)=17632
                a(n)=38*a(n-1)-533*a(n-2)+3268*a(n-3)-7392*a(n-4)   (n≥4)
   Coefficients come from expanding the denominator.
   The computation uses only integer arithmetic, works exactly on
   arbitrary‑precision seq_t, and fits into uint64_t for all terms that
   fit in that type.  */

seq_t a(idx_t n)
{
    /* Handle the small initial values explicitly. */
    if (n == 0) return 1;
    if (n == 1) return 38;
    if (n == 2) return 911;
    if (n == 3) return 17632;

    /* Rolling window for the last four values. */
    seq_t a0 = 1, a1 = 38, a2 = 911, a3 = 17632;

    for (idx_t i = 4; i <= n; ++i)
    {
        /* 38*a3  - 533*a2 + 3268*a1 - 7392*a0  */
        seq_t next = 38 * a3
                   - 533 * a2
                   + 3268 * a1
                   - 7392 * a0;

        /* Shift the window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
