/*
 * A160517  A walk of 10-divisible "less regular" figurate cuboctahedra, from
 *          sequence A160249.
 * https://oeis.org/A160517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl, uned
 * Formula:   a(n) = 10 * ( 2*C(floor((n-1)/2)+2,3) + * (n%2==0 ?
 *            2*T(floor((n-1)/2)+1) : T(floor((n-1)/2)+1) ) ) * where C(k,3) =
 *            k*(k-1)*(k-2)/6 and T(k) = k*(k+1)/2
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1, n=0 is unused */

    /* k = floor((n-1)/2) */
    idx_t k = (n - 1) / 2;

    /* Compute C(k+2,3) = (k+2)(k+1)k / 6 */
    seq_t kp2 = (seq_t)(k + 2);
    seq_t kp1 = (seq_t)(k + 1);
    seq_t km  = (seq_t)k;
    seq_t beta = kp2 * kp1 * km / 6;      /* tetrahedral number */

    /* Compute T(k+1) = (k+1)(k+2)/2 */
    seq_t tri = kp1 * kp2 / 2;            /* triangular number */

    /* Compute a(n) */
    seq_t part_tri = (n & 1ULL) == 0 ? (seq_t)2 * tri : tri;   /* even -> 2*tri, odd -> tri */
    seq_t result = (seq_t)10 * ( (seq_t)2 * beta + part_tri );
    return result;
}
