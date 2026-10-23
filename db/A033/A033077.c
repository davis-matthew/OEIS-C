/*
 * A033077  Numbers in which all pairs of consecutive base-6 digits differ by
 *          3.
 * https://oeis.org/A033077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 6*a(n-5) + a(n-10) - 6*a(n-15) for n > 15 Initial values:
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=5, a(6)=10, a(7)=17,
 *            a(8)=18, a(9)=25, a(10)=32, a(11)=61, a(12)=104, a(13)=111,
 *            a(14)=154, a(15)=197
 * Range:     u8 n <= 15, u16 n <= 30, u32 n <= 61, u64 n <= 123,
 *            u128 n <= 246, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t init[15] = {
    (seq_t)1,  (seq_t)2,  (seq_t)3,  (seq_t)4,  (seq_t)5,
    (seq_t)10, (seq_t)17, (seq_t)18, (seq_t)25, (seq_t)32,
    (seq_t)61, (seq_t)104, (seq_t)111, (seq_t)154, (seq_t)197
};

seq_t a(idx_t n)
{
    /* For n <= 15 just return the table value. */
    if (n <= 15)
        return init[n - 1];

    /* Buffer of the last 15 terms. */
    seq_t buf[15];
    for (int i = 0; i < 15; ++i)
        buf[i] = init[i];

    /* Compute from 16 up to n using the recurrence.
       Index mapping: term k is stored at buf[(k-1)%15]. */
    for (idx_t k = 16; k <= n; ++k) {
        seq_t v1 = ((seq_t)6) * buf[(k - 5 - 1) % 15];
        seq_t v2 = buf[(k - 10 - 1) % 15];
        seq_t v3 = ((seq_t)6) * buf[(k - 15 - 1) % 15];
        seq_t val = v1 + v2 - v3;
        buf[(k - 1) % 15] = val;
    }

    return buf[(n - 1) % 15];
}
