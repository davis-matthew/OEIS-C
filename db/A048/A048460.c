/*
 * A048460  Total of odd numbers in the generations from 2 onwards.
 * https://oeis.org/A048460
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed from the decomposition n = odd · 2^p where odd
 *            is odd and p ≥ 0. Then a(n) = (2^(p-1)+1) · A001316((odd-1)/2),
 *            p > 0 a(n) = (3/2) · A001316((odd-1)/2), p = 0 where A001316 is
 *            defined by A001316(0) = 1 A001316(k) = A001316(k/2) if k even
 *            A001316(k) = 2·A001316((k-1)/2) if k odd
 * Range:     u8 n <= 509, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n ≥ 2 */
    idx_t temp = n, p = 0;

    /* count trailing zeros to find p */
    while ((temp & 1) == 0) {
        temp >>= 1;
        ++p;
    }

    idx_t odd = temp;                 /* odd part of n  */
    idx_t k   = (odd - 1) >> 1;        /* (odd-1)/2      */

    /* compute A001316(k) iteratively */
    seq_t val = 1;
    while (k > 0) {
        if (k & 1) {                  /* k odd */
            val *= 2;
            k = (k - 1) >> 1;
        } else {                      /* k even */
            k >>= 1;
        }
    }

    if (p == 0) {                     /* n odd */
        return ((seq_t)3 * val) / 2;   /* 1.5 * val but val is even */
    } else {                          /* n even */
        seq_t factor = ((seq_t)1 << (p - 1)) + 1; /* 2^(p-1)+1 */
        return val * factor;
    }
}
