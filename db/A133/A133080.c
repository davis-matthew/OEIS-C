/*
 * A133080  Interpolation operator: Triangle with an even number of zeros in
 *          each line followed by 1 or 2 ones.
 * https://oeis.org/A133080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = 1 if the position in the triangle is on the main diagonal
 *            or on the sub‑diagonal of an even row, otherwise 0
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for 128‑bit unsigned values:
   returns floor(sqrt(x)).  Uses binary search on 64‑bit range. */
static inline uint64_t isqrt128(unsigned __int128 x)
{
    uint64_t low = 0, high = UINT64_MAX;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;            /* floor(sqrt(x)) */
}

/* Sequence A133080 */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* undefined for n=0, safety. */

    /* Determine the row r such that r*(r+1)/2 >= n. */
    unsigned __int128 d   = (unsigned __int128)n * (unsigned __int128)8 + 1;
    uint64_t      s       = isqrt128(d);          /* floor(sqrt(8n+1)) */
    uint64_t      r       = (s - 1) >> 1;          /* initial approx */

    /* Correct r if necessary (since we used floor sqrt). */
    unsigned __int128 tri = (unsigned __int128)r * (r + 1) / 2;
    if (tri < n) ++r;

    /* Position of n within row r (1‑based). */
    unsigned __int128 prior = (unsigned __int128)(r - 1) * r / 2;
    uint64_t       pos     = (uint64_t)(n - prior);

    /* Determine value: 1 on main diagonal or even row sub‑diagonal, else 0. */
    if (pos == r) return (seq_t)1;                /* main diagonal */
    if (pos == r - 1 && (r & 1u) == 0) return (seq_t)1; /* even‑row sub‑diagonal */
    return (seq_t)0;                               /* all other positions */
}
