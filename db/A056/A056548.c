/*
 * A056548  a(n) = Sum_{k>=1} round(n/k) where round(1/2) = 0.
 * https://oeis.org/A056548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k>=1} round(n/k) where round(1/2)=0 * (round(x) =
 *            nearest integer, ties downwards)
 * Range:     u8 n <= 47, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t sum = 0;
    /* k from 1 to n */
    for (idx_t k = 1; k <= n; ++k) {
        idx_t q = n / k;
        idx_t r = n % k;
        /* round: if 2*r > k round up, else round down (ties go down) */
        if ((r << 1) > k)
            sum += (seq_t)(q + 1);
        else
            sum += (seq_t)q;
    }
    /* k from n+1 to 2*n-1 */
    idx_t double_n = n + n;          /* 2*n, safe here because n is small (≤10000) */
    for (idx_t k = n + 1; k < double_n; ++k) {   /* k < 2*n ensures last term is k = 2*n-1 */
        idx_t q = n / k;
        idx_t r = n % k;
        if ((r << 1) > k)
            sum += (seq_t)(q + 1);
        else
            sum += (seq_t)q;
    }

    return sum;
}
