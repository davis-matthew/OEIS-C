/*
 * A240871  Number of partitions p of n into distinct parts such that max(p) =
 *          3 + min(p).
 * https://oeis.org/A240871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9375, u16 n <= 9375, u32 n <= 9375, u64 n <= 9375,
 *            u128 n <= 9375, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = a(n - 12) for n ≥ 19,
 * with initial values
 * a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=0,
 * a(5)=1, a(6)=0, a(7)=2, a(8)=1, a(9)=1,
 * a(10)=2, a(11)=2, a(12)=0, a(13)=2,
 * a(14)=2, a(15)=1, a(16)=1, a(17)=2,
 * a(18)=1
 *
 * The recurrence yields a periodic sequence of period 12 for n ≥ 19.
 */

seq_t a(idx_t n)
{
    /* precomputed initial values for n = 0 … 18 */
    static const seq_t init[] = {
        (seq_t)0, /* 0 */
        (seq_t)0, /* 1 */
        (seq_t)0, /* 2 */
        (seq_t)0, /* 3 */
        (seq_t)0, /* 4 */
        (seq_t)1, /* 5 */
        (seq_t)0, /* 6 */
        (seq_t)2, /* 7 */
        (seq_t)1, /* 8 */
        (seq_t)1, /* 9 */
        (seq_t)2, /*10*/
        (seq_t)2, /*11*/
        (seq_t)0, /*12*/
        (seq_t)2, /*13*/
        (seq_t)2, /*14*/
        (seq_t)1, /*15*/
        (seq_t)1, /*16*/
        (seq_t)2, /*17*/
        (seq_t)1  /*18*/
    };
    const idx_t init_len = sizeof(init)/sizeof(init[0]);

    if (n < init_len)
        return init[n];

    /* Apply the recurrence: a(n) = a(n-12) for n ≥ 19 */
    while (n >= 19)
        n -= 12;

    return init[n];
}
