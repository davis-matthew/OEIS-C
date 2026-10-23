/*
 * A021129  Expansion of 1/((1-x)*(1-2*x)*(1-5*x)*(1-8*x)).
 * https://oeis.org/A021129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=16, a(2)=175, a(3)=1650;
   for n>3, a(n)=16*a(n-1)-81*a(n-2)+146*a(n-3)-80*a(n-4) */

seq_t a(idx_t n)
{
    /* Handle the first four known terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)16;
    if (n == 2) return (seq_t)175;
    if (n == 3) return (seq_t)1650;

    /* Setup the four previous values */
    seq_t a_nm4 = (seq_t)1;    /* a(n-4) */
    seq_t a_nm3 = (seq_t)16;   /* a(n-3) */
    seq_t a_nm2 = (seq_t)175;  /* a(n-2) */
    seq_t a_nm1 = (seq_t)1650; /* a(n-1) */
    seq_t en;                 /* a(n) */

    /* Iterate up to the requested n */
    for (idx_t i = 4; i <= n; ++i) {
        en = (seq_t)16 * a_nm1
            - (seq_t)81 * a_nm2
            + (seq_t)146 * a_nm3
            - (seq_t)80 * a_nm4;

        /* Shift the window of previous terms */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = en;
    }

    return a_nm1;
}
