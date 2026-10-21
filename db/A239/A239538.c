/*
 * A239538  Number of (4+1)X(n+1) 0..2 arrays with no element equal to all
 *          horizontal neighbors or unequal to all vertical neighbors, and new
 *          values 0..2 introduced in row major order.
 * https://oeis.org/A239538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1)+46*a(n-2)-22*a(n-3)-166*a(n-4)+28*a(n-5)
 *            +68*a(n-6)+208*a(n-7)-160*a(n-8)-48*a(n-9)+32*a(n-10)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values a(1) .. a(10) are taken from the OEIS entry. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* offset is 1; a(0) is unused */

    /* First 10 terms – required initial conditions for the recurrence. */
    static const seq_t init[10] = {
        13ULL,          /* a(1)  */
        68ULL,          /* a(2)  */
        1014ULL,        /* a(3)  */
        11108ULL,       /* a(4)  */
        131988ULL,      /* a(5)  */
        1533792ULL,     /* a(6)  */
        17931608ULL,    /* a(7)  */
        209295248ULL,   /* a(8)  */
        2443954640ULL,  /* a(9)  */
        28534764096ULL  /* a(10) */
    };

    if (n <= 10)
        return init[n - 1];

    /* Circular buffer holding the last ten computed values:
       buf[0] = a(i-10), buf[1] = a(i-9), …, buf[9] = a(i-1)   */
    seq_t buf[10];
    for (idx_t k = 0; k < 10; ++k)
        buf[k] = init[k];

    for (idx_t i = 11; i <= n; ++i) {
        /* Compute the next term using only additions and one subtraction.
           The subtraction is safe because the recurrence guarantees
           the positive sum > negative sum for all n. */
        seq_t pos = 8ULL   * buf[9]    /* a(i-1) */
                + 46ULL  * buf[8]    /* a(i-2) */
                + 28ULL  * buf[5]    /* a(i-5) */
                + 68ULL  * buf[4]    /* a(i-6) */
                + 208ULL * buf[3]    /* a(i-7) */
                + 32ULL  * buf[0];   /* a(i-10) */

        seq_t neg = 22ULL  * buf[7]    /* a(i-3) */
                + 166ULL * buf[6]    /* a(i-4) */
                + 160ULL * buf[2]    /* a(i-8) */
                + 48ULL  * buf[1];   /* a(i-9) */

        seq_t next = pos - neg;

        /* Slide the buffer left and insert the new term. */
        for (idx_t k = 0; k < 9; ++k)
            buf[k] = buf[k + 1];
        buf[9] = next;
    }

    return buf[9];
}
