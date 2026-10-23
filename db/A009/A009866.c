/*
 * A009866  Coordination sequence T1 for Zeolite Code AHT.
 * https://oeis.org/A009866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Recurrence a(n) = 2*a(n-1) - a(n-2) + a(n-8) - 2*a(n-9) +
 *            a(n-10) with initial values a(0)=1, a(1)=4, a(2)=11, a(3)=21,
 *            a(4)=36, a(5)=56, a(6)=81, a(7)=109, a(8)=142, a(9)=179,
 *            a(10)=221 (the recurrence holds for n ≥ 11)
 * Range:     u8 n <= 10, u16 n <= 173, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* First 11 terms directly from the OEIS sequence */
    static const seq_t init[11] = {
        (seq_t)1, (seq_t)4, (seq_t)11, (seq_t)21, (seq_t)36,
        (seq_t)56, (seq_t)81, (seq_t)109, (seq_t)142, (seq_t)179,
        (seq_t)221
    };

    if (n < 11)
        return init[n];

    /* Buffer holding the last 11 computed terms.
       buf[10] will always contain the most recent a(k).  */
    seq_t buf[11];
    for (idx_t i = 0; i < 11; ++i)
        buf[i] = init[i];

    /* Iterate from 11 up to n, updating the buffer each step. */
    for (idx_t k = 11; k <= n; ++k) {
        /* recurrence using elements of the buffer:
           buf[10] -> a(k-1)
           buf[9]  -> a(k-2)
           buf[3]  -> a(k-8)
           buf[2]  -> a(k-9)
           buf[1]  -> a(k-10) */
        seq_t next =  (seq_t)2 * buf[10]       /* 2*a(k-1)   */
                    -              buf[9]    /* -a(k-2)     */
                    +              buf[3]    /* +a(k-8)     */
                    - (seq_t)2 * buf[2]       /* -2*a(k-9)   */
                    +              buf[1];   /* +a(k-10)    */

        /* Shift the buffer: discard a(k-11), shift left */
        for (idx_t i = 0; i < 10; ++i)
            buf[i] = buf[i + 1];
        buf[10] = next;
    }

    return buf[10];
}
