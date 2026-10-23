/*
 * A127002  Number of partitions of n that have the form a+a+b+c where a,b,c
 *          are distinct.
 * https://oeis.org/A127002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(floor((n-1)/2),2) - floor((n-1)/3) -
 *            floor((n-1)/4) + floor(n/4)
 * Range:     u8 n <= 48, u16 n <= 728, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute the terms of the formula using signed 64‑bit arithmetic;
       these values are safely within the 64‑bit range for n up to 10⁴. */
    long long k   = (long long)(n - 1) / 2;          /* floor((n-1)/2)   */
    long long bin = k * (k - 1) / 2;                 /* binomial(k,2)    */
    long long f3  = (long long)(n - 1) / 3;          /* floor((n-1)/3)   */
    long long f4  = (long long)(n - 1) / 4;          /* floor((n-1)/4)   */
    long long f4n = (long long) n / 4;               /* floor(n/4)       */

    long long res = bin - f3 - f4 + f4n;             /* final result     */
    /* res is guaranteed non‑negative for all n≥1 */
    return (seq_t)res;
}
