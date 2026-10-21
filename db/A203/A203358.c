/*
 * A203358  Number of n X 4 binary arrays with every 1 immediately preceded by
 *          0 to the left or above.
 * https://oeis.org/A203358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 42*a(n-2) + 54*a(n-3) + 23*a(n-4) - 7*a(n-5)
 *            Initial values: a(1)=5, a(2)=61, a(3)=565, a(4)=5677, a(5)=55517
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[6] = { (seq_t)0,
                                  (seq_t)5,
                                  (seq_t)61,
                                  (seq_t)565,
                                  (seq_t)5677,
                                  (seq_t)55517 };
    if (n <= 5)
        return init[n];

    seq_t a1 = init[1];
    seq_t a2 = init[2];
    seq_t a3 = init[3];
    seq_t a4 = init[4];
    seq_t a5 = init[5];
    idx_t i;

    for (i = 6; i <= n; ++i)
    {
        seq_t next = (seq_t)5 * a5 + (seq_t)42 * a4 + (seq_t)54 * a3 + (seq_t)23 * a2;
        next -= (seq_t)7 * a1;
        a1 = a2; a2 = a3; a3 = a4; a4 = a5; a5 = next;
    }

    return a5;
}
