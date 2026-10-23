/*
 * A041034  Numerators of continued fraction convergents to sqrt(22).
 * https://oeis.org/A041034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 394*a(n-6) - a(n-12); Initial values: a(0)=4, a(1)=5,
 *            a(2)=14, a(3)=61, a(4)=136, a(5)=197, a(6)=1712, a(7)=1909,
 *            a(8)=5530, a(9)=24029, a(10)=53588, a(11)=77617
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Three special cases: normal unsigned integer classification. */
    static const seq_t init[12] = {
        4,   5,   14,  61,  136, 197,
        1712,1909,5530,24029,53588,77617
    };

    /* Return the initial values directly for n < 12 */
    if (n < 12)
        return init[(int)n];

    /* Ring buffer of the last 12 computed terms */
    seq_t buf[12];
    for (int i = 0; i < 12; ++i)
        buf[i] = init[i];

    /* Compute each term from 12 up to n using the recurrence */
    for (idx_t i = 12; i <= n; ++i) {
        seq_t mult = 394 * buf[(i - 6) % 12];
        seq_t val  = mult - buf[(i - 12) % 12];
        buf[i % 12] = val;
    }

    return buf[(int)(n % 12)];
}
