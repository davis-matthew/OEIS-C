/*
 * A088207  a(n) = Sum_{k=0..n} floor(k*phi^2) where phi=(1+sqrt(5))/2.
 * https://oeis.org/A088207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} floor(k*phi^2), with phi=(1+sqrt5)/2
 * Range:     u8 n <= 6, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 49 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: floor sqrt(5*k^2).  Uses only integer arithmetic. */
static seq_t sqrt5k2(idx_t k)
{
    seq_t kk = (seq_t)k;
    seq_t s = kk * kk;
    s *= (seq_t)5;

    /* Binary search for floor sqrt(s).  High bound is 3*k+1 > sqrt(5)*k. */
    seq_t low  = 0;
    seq_t high = kk * (seq_t)3 + 1;

    while (low < high) {
        seq_t mid  = (low + high) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= s)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;   /* low is the first > floor sqrt(s) */
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t kk       = (seq_t)k;
        seq_t sqrtk5   = sqrt5k2(k);
        seq_t floor_k_phi   = (kk + sqrtk5) / 2;   /* floor(k*phi) */
        seq_t floor_k_phi2  = kk + floor_k_phi;    /* floor(k*phi^2) = k + floor_k_phi */
        sum += floor_k_phi2;
    }
    return sum;   /* n==0 returns 0 automatically */
}
