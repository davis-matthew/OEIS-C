/*
 * A023054  Simon Plouffe's conjectured extension of sequence A008368.
 * https://oeis.org/A023054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  Sequence A023054
 *
 *  Generating function:  (1 - x^5) / ((1 - x) * (1 - x^2)^2 * (1 - x^3))
 *
 *  The denominator expands to  
 *      1 - x - 2x^2 + x^3 + 2x^4 + x^5 - 2x^6 - x^7 + x^8,
 *  giving the linear recurrence (for n ≥ 8):
 *
 *      a(n) =  a(n-1) + 2a(n-2) - a(n-3) - 2a(n-4) - a(n-5)
 *            + 2a(n-6) + a(n-7) - a(n-8)
 *
 *  Initial values (n = 0 … 7):
 *      1, 1, 3, 4, 7, 8, 13, 14
 */

seq_t a(idx_t n)
{
    /* Initial seven terms – keep as seq_t constants. */
    static const seq_t init[8] = {
        (seq_t)1,  (seq_t)1,  (seq_t)3,
        (seq_t)4,  (seq_t)7,  (seq_t)8,
        (seq_t)13, (seq_t)14
    };

    if (n < 8)
        return init[(int)n];

    /* Ring buffer holding the last eight values:
     *   ring[0] = a(i-1), ring[1] = a(i-2), …, ring[7] = a(i-8)
     */
    seq_t ring[8];
    for (idx_t k = 0; k < 8; ++k)
        ring[k] = init[7 - k];   /* fill with a(7)…a(0) */

    for (idx_t i = 8; i <= n; ++i) {
        seq_t next = ring[0]                       /* a(i-1)   */
                    + (seq_t)2 * ring[1]           /* 2a(i-2)  */
                    - ring[2]                     /* -a(i-3)  */
                    - (seq_t)2 * ring[3]           /* -2a(i-4) */
                    - ring[4]                     /* -a(i-5)  */
                    + (seq_t)2 * ring[5]           /* +2a(i-6) */
                    + ring[6]                     /* +a(i-7)  */
                    - ring[7];                    /* -a(i-8)  */

        /* Shift ring right by one position, insert new value. */
        for (idx_t k = 7; k > 0; --k)
            ring[k] = ring[k-1];
        ring[0] = next;
    }

    return ring[0];   /* after the loop ring[0] = a(n) */
}
