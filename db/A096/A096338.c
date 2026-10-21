/*
 * A096338  a(n) = (2/(n-1))*a(n-1) + ((n+5)/(n-1))*a(n-2) with a(0)=0 and
 *          a(1)=1.
 * https://oeis.org/A096338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (⌊n/2⌋+1)*(⌊n/2⌋+2)*(⌊n/2⌋+3)*(2*n-3*⌊n/2⌋)/12 (all
 *            arithmetic done in unsigned long long to avoid overflow)
 * Range:     u8 n <= 12, u16 n <= 56, u32 n <= 949, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned long long m      = n / 2;
    unsigned long long t1     = m + 1;
    unsigned long long t2     = m + 2;
    unsigned long long t3     = m + 3;
    unsigned long long factor = 2 * n - 3 * m;   /* 2*n-3*⌊n/2⌋ is always non‑negative */

    unsigned long long prod  = t1;
    prod                    *= t2;
    prod                    *= t3;
    prod                    *= factor;          /* prod fits in 64 bits for all n used in tests */

    unsigned long long res  = prod / 12;          /* division is exact */

    return (seq_t)res;                           /* cast to the sequence type */
}
