/*
 * A025699  Index of 3^n within the sequence of the numbers of the form
 *          3^i*8^j (A025615).
 * https://oeis.org/A025699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Sum_{k=0..n} ceil(k * log_8(3))
 * Range:     u8 n <= 29, u16 n <= 496, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (uses an equivalent counting formulation) */
#include <stdint.h>

static const unsigned long long LN3_SCAL = 1098612288668109691ULL; /* ln(3) * 1e18 */
static const unsigned long long LN8_SCAL = 2079441541679835928ULL; /* ln(8) * 1e18 */

seq_t a(idx_t n)
{
    /* a(0) is the first term of the sequence */
    if (n == 0)
        return (seq_t)1;

    unsigned long long n64 = (unsigned long long)n;

    /* Jmax = floor(n / t) where t = ln(8)/ln(3) ; equivalently */
    unsigned __int128 prodJ = (unsigned __int128)n64 * LN3_SCAL;
    unsigned long long Jmax = (unsigned long long)(prodJ / LN8_SCAL);

    /* Sum_{j=0}^{Jmax} floor(j * t) where t = ln(8)/ln(3) .
       Using integer arithmetic: floor(j * t) = (j * LN8_SCAL) / LN3_SCAL   */
    unsigned __int128 sum_floor = 0;
    for (unsigned long long j = 0; j <= Jmax; ++j) {
        unsigned __int128 prodF = (unsigned __int128)j * LN8_SCAL;
        unsigned long long floor_val = (unsigned long long)(prodF / LN3_SCAL);
        sum_floor += floor_val;
    }

    /* a(n) = (Jmax + 1) * n + 1 - sum_floor  (derived from counting) */
    unsigned __int128 term = (unsigned __int128)(Jmax + 1) * n64 + 1 - sum_floor;
    return (seq_t)(unsigned long long)term;
}
