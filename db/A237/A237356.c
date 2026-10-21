/*
 * A237356  The number of tilings of the 3 X 4 X (2n) room with 1 X 2 X 2
 *          boxes.
 * https://oeis.org/A237356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 55*a(n-1) - 700*a(n-2) + 3186*a(n-3) - 5180*a(n-4) +
 *            2640*a(n-5) for n ≥ 5, with a(0)=1, a(1)=29, a(2)=1065,
 *            a(3)=41097, a(4)=1602289
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial terms (n = 0 to 4) ------------------------------------------------- */
    static const seq_t init[5] = {
        (seq_t)1,          /* a(0) */
        (seq_t)29,         /* a(1) */
        (seq_t)1065,       /* a(2) */
        (seq_t)41097,      /* a(3) */
        (seq_t)1602289     /* a(4) */
    };

    if (n < 5)
        return init[n];

    /* Initialize the window of last five terms in *descending* order:
       a0 = a(n-1), a1 = a(n-2), …, a4 = a(n-5). The initial step starts with
       n = 5, so we load a(4), a(3), a(2), a(1), a(0). ------------------------ */
    seq_t a0 = init[4];      /* a(4) */
    seq_t a1 = init[3];      /* a(3) */
    seq_t a2 = init[2];      /* a(2) */
    seq_t a3 = init[1];      /* a(1) */
    seq_t a4 = init[0];      /* a(0) */

    /* Iterate from n = 5 up to the desired n --------------------------------- */
    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = a0 * (seq_t)55;      /* +55*a(n-1) */
        next += a2 * (seq_t)3186;         /* +3186*a(n-3) */
        next += a4 * (seq_t)2640;         /* +2640*a(n-5) */
        next -= a1 * (seq_t)700;          /* -700*a(n-2) */
        next -= a3 * (seq_t)5180;         /* -5180*a(n-4) */

        /* Slide the window: a(n) becomes the new a0, etc. ------------------- */
        a4 = a3;
        a3 = a2;
        a2 = a1;
        a1 = a0;
        a0 = next;
    }

    return a0;   /* a(n) */
}
