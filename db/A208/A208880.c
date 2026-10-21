/*
 * A208880  Number of words either empty or beginning with the first letter of
 *          the cyclic n-ary alphabet, where each letter of the alphabet
 *          occurs twice and letters of neighboring word positions are equal
 *          or neighbors in the alphabet.
 * https://oeis.org/A208880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 40, u64 n <= 86, u128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(2)=3, a(3)=30,
   a(4)=62, a(5)=114, a(6)=202;
   for n ≥ 7 : a(n) = 3*a(n-1) - 2*a(n-2) - a(n-3) + a(n-4) */
seq_t a(idx_t n)
{
    /* base values */
    static const seq_t init[7] =
    {
        (seq_t)1,  /* a(0) */
        (seq_t)1,  /* a(1) */
        (seq_t)3,  /* a(2) */
        (seq_t)30, /* a(3) */
        (seq_t)62, /* a(4) */
        (seq_t)114,/* a(5) */
        (seq_t)202 /* a(6) */
    };

    if (n <= 6)
        return init[n];

    /* last four terms needed for the recurrence */
    seq_t a_nm4 = init[3];   /* a(n-4) when n = 7  -> a(3) */
    seq_t a_nm3 = init[4];   /* a(4) */
    seq_t a_nm2 = init[5];   /* a(5) */
    seq_t a_nm1 = init[6];   /* a(6) */
    seq_t a_n;

    for (idx_t i = 7; i <= n; ++i)
    {
        a_n = 3 * a_nm1 - 2 * a_nm2 - a_nm3 + a_nm4;
        /* shift the window forward */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_nm1;
}
