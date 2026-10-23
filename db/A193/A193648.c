/*
 * A193648  T(n,k)=Number of arrays of -k..k integers x(1..n) with every x(i)
 *          being in a substring of length 1 or 2 with sum zero. Array listed
 *          by antidiagonals.
 * https://oeis.org/A193648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(p, s-p) where * s = minimal integer with (s-1)*s/2 ≥ n,
 *            * p = n - (s-2)*(s-1)/2 (1 ≤ p ≤ s-1), * T(n,k) satisfies
 *            T(1,k)=1, T(2,k)=2*k+1, T(3,k)=6*k+1, * and for n≥4:
 *            T(n,k)=2*T(n-1,k)+2*(k-1)*T(n-2,k)+T(n-3,k)
 * Range:     u8 n <= 26, u16 n <= 74, u32 n <= 224, u64 n <= 656,
 *            u128 n <= 1939, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Determine diagonal index s and position p */
    idx_t s = 2;
    idx_t cumulative = 1;           /* (s-1)*s/2 for current s */
    while (cumulative < n) {
        ++s;
        cumulative += s - 1;
    }
    idx_t prev = cumulative - (s - 1);    /* (s-2)*(s-1)/2 */
    idx_t p = n - prev;                   /* 1 … s-1 */
    idx_t k = s - p;                      /* column index */
    idx_t nT = p;                         /* row index for T */

    /* Compute T(nT, k) with the recurrence */
    seq_t k_val = (seq_t)k;
    seq_t k_minus1 = k_val - 1;

    if (nT == 1) return (seq_t)1;

    seq_t t_prev3 = (seq_t)1;          /* T(1,k) */
    seq_t t_prev2 = 2 * k_val + 1;     /* T(2,k) */
    if (nT == 2) return t_prev2;

    seq_t t_prev1 = 6 * k_val + 1;     /* T(3,k) */
    if (nT == 3) return t_prev1;

    for (idx_t i = 4; i <= nT; ++i) {
        seq_t cur = 2 * t_prev1 + 2 * k_minus1 * t_prev2 + t_prev3;
        t_prev3 = t_prev2;
        t_prev2 = t_prev1;
        t_prev1 = cur;
    }
    return t_prev1;
}
