/*
 * A081750  Simple continued fraction of 2*e.
 * https://oeis.org/A081750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cofr, easy, nonn
 * Formula:   a(0)=5, a(1)=2, a(2)=3, a(3)=2, a(4)=3, a(5)=1. For n ≥ 6 let k
 *            = n / 6 and r = n % 6. Then: a(n) = 2k if r == 0 1 if r == 1 or
 *            r == 5 3 if r == 2 or r == 4 2k + 2 if r == 3 (Equivalent to
 *            a(6k)=2k, a(6k+1)=1, a(6k+2)=3, a(6k+3)=2k+2, a(6k+4)=3,
 *            a(6k+5)=1.)
 * Range:     u8 n <= 764, u16 n <= 7449, u32 n <= 7449, u64 n <= 7449,
 *            u128 n <= 7449, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[6] = {5, 2, 3, 2, 3, 1};

    if (n < 6)
        return init[n];

    idx_t k = n / 6;
    idx_t r = n % 6;
    seq_t kseq = (seq_t)k;        /* cast index to seq_t for arithmetic */

    switch (r) {
    case 0:
        return kseq * 2;
    case 1:
    case 5:
        return (seq_t)1;
    case 2:
    case 4:
        return (seq_t)3;
    case 3:
        return kseq * 2 + (seq_t)2;
    default:
        /* Should never happen */
        return (seq_t)0;
    }
}
