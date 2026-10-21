/*
 * A209647  Number of n X 5 0..1 arrays avoiding 0 0 1 and 1 0 0 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A209647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (7/2)*n^4 + 21*n^3 - (7/2)*n^2 - 7*n (for n ≥ 1, a(0)=0)
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 185, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is outside the definition range – return 0.   */
    if (n == 0u)
        return 0;

    /* Use 64‑bit arithmetic for intermediate values to avoid
       overflow on platforms where seq_t is smaller.          */
    unsigned long long sn   = (unsigned long long)n;
    unsigned long long n2   = sn * sn;          /* n²   */
    unsigned long long n4   = n2 * n2;          /* n⁴   */
    unsigned long long term1 = ((unsigned long long)7 * (n4 - n2)) / 2;   /* (7/2)(n⁴‑n²) */
    unsigned long long n3   = n2 * sn;          /* n³   */
    unsigned long long term3 = (unsigned long long)21 * n3;              /* 21n³   */
    unsigned long long term4 = (unsigned long long)7 * sn;              /* 7n     */

    unsigned long long result = term1 + term3 - term4;
    return (seq_t)result;
}
