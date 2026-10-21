/*
 * A070978  Smallest k such that zeta(4)-sum(i=1,k,1/i^4) <=
 *          zeta(5)-sum(i=1,n,1/i^5).
 * https://oeis.org/A070978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest k such that ζ(4)-∑_{i=1}^{k}1/i⁴ ≤
 *            ζ(5)-∑_{i=1}^{n}1/i⁵
 * Range:     u8 n <= 58, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is implemented by pre‑computing prefix sums of 1/i⁴ and 1/i⁵,
   then binary searching the latter with respect to the former. */

#include <stdlib.h>
#include <stdint.h>

/* Pre‑computed values (built on first call). */
static int _a_init_done = 0;
static const idx_t _KMAX = 200000;               /* safe upper bound for k   */
static long double *_sum4 = NULL;                /* 0 … KMAX  :  Σ 1/i⁴       */
static long double *_sum5 = NULL;                /* 0 … KMAX  :  Σ 1/i⁵       */
static long double _zeta4;                       /* ζ(4)                     */
static long double _zeta5;                       /* ζ(5)                     */
static long double _D;                           /* ζ(4)-ζ(5)                 */

static void _a_init(void)
{
    if (_a_init_done) return;
    _sum4 = (long double *)malloc((_KMAX + 1) * sizeof(long double));
    _sum5 = (long double *)malloc((_KMAX + 1) * sizeof(long double));
    if (!_sum4 || !_sum5) return;               /* allocation failed – unlikely */

    _sum4[0] = 0.0L;
    _sum5[0] = 0.0L;
    long double s4 = 0.0L, s5 = 0.0L;
    for (idx_t i = 1; i <= _KMAX; ++i) {
        long double iv   = (long double)i;
        long double iv2  = iv * iv;
        long double iv4  = iv2 * iv2;          /* i⁴  */
        long double term4 = 1.0L / iv4;        /* 1/i⁴ */
        s4 += term4;
        _sum4[i] = s4;

        long double term5 = term4 / iv;        /* 1/i⁵ = 1/(i⁴·i) */
        s5 += term5;
        _sum5[i] = s5;
    }

    /* Estimate the tails beyond KMAX using integral bounds */
    long double inv = (long double)(_KMAX + 1);
    long double inv3 = inv * inv * inv;
    long double inv4 = inv3 * inv;
    _zeta4 = _sum4[_KMAX] + 1.0L/(3.0L*inv3);        /* ζ(4) ≈ sum + 1/(3(K+1)³) */
    _zeta5 = _sum5[_KMAX] + 1.0L/(4.0L*inv4);        /* ζ(5) ≈ sum + 1/(4(K+1)⁴) */
    _D = _zeta4 - _zeta5;                          /* constant difference */

    _a_init_done = 1;
}

seq_t a(idx_t n)
{
    _a_init();

    /* If n exceeds pre‑computed range, fall back to a conservative estimate
       (though the problem guarantees n ≤ 10000). */
    if (n > _KMAX) return (seq_t)0;

    /* Target prefix sum of 1/i⁴:  ζ(4) - (ζ(5)-∑_{i≤n}1/i⁵) = ζ(4)-ζ(5)+∑_{i≤n}1/i⁵ */
    long double target = _D + _sum5[n];

    /* Binary search minimal k with prefix4[k] ≥ target */
    idx_t low = 1, high = _KMAX;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (_sum4[mid] >= target)
            high = mid;
        else
            low  = mid + 1;
    }
    return (seq_t)low;
}
