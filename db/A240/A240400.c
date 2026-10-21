/*
 * A240400  Numbers n having a partition into distinct parts of form 3^k-2^k.
 * https://oeis.org/A240400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(log2(n-1))} 3^k * floor((n-1)/2^k) (n≥1)
 * Range:     u8 n <= 24, u16 n <= 704, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* offset 1: a(1)=0, a(2)=1, a(3)=5, a(4)=6, ... */

seq_t a(idx_t n)
{
    if (n <= 1) {
        return 0;          /* a(1) = 0 */
    }

    idx_t m = n - 1;        /* shift so that n=1 → m=0 */
    unsigned int maxk = 0;
    idx_t t = m;

    /* compute floor(log2(m)) */
    while (t > 1) {
        t >>= 1;
        ++maxk;
    }

    seq_t sum = 0;
    seq_t pow3 = 1;         /* 3^k */

    for (unsigned int k = 0; k <= maxk; ++k) {
        idx_t q = m >> k;               /* floor(m / 2^k) */
        sum += pow3 * (seq_t)q;         /* add 3^k * q */
        pow3 *= 3;                      /* next power of 3 */
    }

    return sum;
}
