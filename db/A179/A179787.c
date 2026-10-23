/*
 * A179787  Let the operation <+> be defined by x<+>y = A038502(x+y). a(n) is
 *          the period in the track of the iterated application
 *          x<+>(x<+>...(x<+>1)) for x = A001651(n-1).
 * https://oeis.org/A179787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the cycle obtained by iterating t →
 *            A038502(A001651(n-1)+t) starting from t = 1, where A001651(k)
 *            lists integers not divisible by 3
 * Range:     u8 n <= 59, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1, unused path */

    /* Compute A001651(n‑1) = the (n‑1)-th positive integer not a multiple of 3 */
    idx_t k      = n - 1;                /* zero‑based index */
    idx_t q      = k >> 1;               /* i.e. k / 2 */
    idx_t r      = k & 1;                /* k % 2 */
    seq_t x = (seq_t)(3 * q + r + 1);    /* 3q+1 for r=0, 3q+2 for r=1 */

    /* Iterate the reduced addition until 1 repeats */
    seq_t cur = 1;
    idx_t step = 0;
    do
    {
        seq_t sum = x + cur;
        while (sum % 3 == 0)
            sum /= 3;
        cur = sum;
        ++step;
    } while (cur != 1);

    return (seq_t)step;
}
