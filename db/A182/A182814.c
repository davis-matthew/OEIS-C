/*
 * A182814  Main diagonal of table A182630.
 * https://oeis.org/A182814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A182814: main diagonal of table A182630.
   For n ≥ 0, a(0)=0.
   If n = T_k = k(k+1)/2 (triangular number) then a(n) = T_k * (k+1).
   Otherwise let m be the largest integer with T_m ≤ n < T_{m+1};
   then a(n) = Σ_{i=1}^{m-1} ((T_{i+1}-T_i-1)*(i+2) + (T_i+1))
              + (T_m+1) + (n-T_m)*(m+2).
   This yields the explicit closed form
   a(n) = (m-1)(m²+2m+2)/2 + T_m + 1 + (n-T_m)(m+2),
   where m = ⌊(√(8n+1)-1)/2⌋. */

#include <stdint.h>

static idx_t triangular_root(idx_t n)
/* largest m with m(m+1)/2 ≤ n */
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t m = triangular_root(n);          /* largest m with T_m ≤ n */
    seq_t Tm = (seq_t)m * (seq_t)(m + 1) / 2;

    /* sum over i = 1 .. m-1 of (3i² + 5i + 2)/2 */
    seq_t sum = 0;
    if (m > 1) {
        seq_t m_seq = (seq_t)m;
        seq_t part = m_seq * m_seq + 2 * m_seq + 2;   /* m² + 2m + 2 */
        sum = (m_seq - 1) * part / 2;                /* (m-1)(m²+2m+2)/2 */
    }

    seq_t term1 = Tm + (seq_t)1;
    seq_t term2 = ((seq_t)n - Tm) * ((seq_t)m + 2);

    return sum + term1 + term2;
}
