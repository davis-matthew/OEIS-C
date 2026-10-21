/*
 * A239909  Arises from a construction of equiangular lines in complex space
 *          of dimension 2.
 * https://oeis.org/A239909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)-a(n-4) with initial values a(1)=1,
 *            a(2)=1, a(3)=2, a(4)=3
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 42, u64 n <= 83, u128 n <= 165,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 4) {
        /* Return one of the first four initial terms directly. */
        static const seq_t init[4] = {
            (seq_t)1,  /* a(1) */
            (seq_t)1,  /* a(2) */
            (seq_t)2,  /* a(3) */
            (seq_t)3   /* a(4) */
        };
        return init[n - 1];
    }

    /* Working window for the recurrence: a0=a(n-4), a1=a(n-3),
       a2=a(n-2), a3=a(n-1). */
    seq_t a0 = (seq_t)1; /* a(1) */
    seq_t a1 = (seq_t)1; /* a(2) */
    seq_t a2 = (seq_t)2; /* a(3) */
    seq_t a3 = (seq_t)3; /* a(4) */
    seq_t next = 0;

    for (idx_t i = 5; i <= n; ++i) {
        /* a(i) = a(i-1)+a(i-2)+a(i-3)-a(i-4) */
        next = a3 + a2 + a1 - a0;
        /* shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3; /* a(n) after the loop */
}
