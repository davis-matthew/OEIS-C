/*
 * A080342  Number of weighings required to identify a single bad coin out of
 *          n coins, using a two-pan balance.
 * https://oeis.org/A080342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(log_3(n)) – number of weighings needed to identify a
 *            single heavier coin among n coins using a two‐pan balance.
 *            Offsets: a(1)=0, a(2)=1, a(3)=1, a(4)=2, …
 * Range:     u8 n <= 1389, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to seq_t for arithmetic with seq_t values */
    seq_t sn = n;
    seq_t pow = 1;      /* current power of 3 (sequence value) */
    idx_t k = 0;        /* exponent: 3^k <= n */

    /* Find the greatest power of 3 not exceeding n */
    while (pow <= sn / 3) {
        pow *= 3;
        ++k;
    }

    /* If n is exactly a power of 3, the ceil is k; otherwise k+1 */
    if (sn == pow) {
        return (seq_t)k;
    } else {
        return (seq_t)(k + 1);
    }
}
