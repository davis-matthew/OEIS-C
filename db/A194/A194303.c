/*
 * A194303  Triangular array: g(n,k)=number of fractional parts (i*sqrt(5)) in
 *          interval [(k-1)/n, k/n], for 1<=i<=n^2, 1<=k<=n.
 * https://oeis.org/A194303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = triangular array value g(r,k) where * r is the row such
 *            that r(r-1)/2 < n ≤ r(r+1)/2 * k = n - r(r-1)/2 * g(r,k) =
 *            number of i (1 ≤ i ≤ r²) whose fractional part of i*√5 lies in *
 *            the interval [(k-1)/r , k/r)
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

static unsigned __int128 const SCALE = (unsigned __int128)1 << 60;
static uint64_t const MASK   = ((uint64_t)1 << 60) - 1;
static const unsigned __int128 A128 = (unsigned __int128)(sqrtl((long double)5.0L) * (long double)(unsigned __int128)SCALE + 0.5L);
static const uint64_t INC   = (uint64_t)(A128 & MASK);
static const uint64_t FLOOR_VAL = (uint64_t)(A128 >> 60);   /* equals 2 for √5 */

seq_t a(idx_t idx) {
    if (idx == 0) return (seq_t)0;          /* index 0 is outside OEIS offset */

    /* ---- find row r ---- */
    /* r satisfies r*(r+1)/2 ≥ idx   and   (r-1)*r/2 < idx   */
    long double d = sqrtl((long double)(8.0L * idx + 1.0L));
    uint64_t r = (uint64_t)((d - 1.0L) / 2.0L);   /* floor of the expression */
    while ((unsigned __int128)r * (r + 1) / 2 < idx) ++r;
    while (r > 1 && (unsigned __int128)(r - 1) * r / 2 >= idx) --r;

    /* ---- position within the row ---- */
    uint64_t prev_sum = (unsigned __int128)(r - 1) * r / 2;
    uint64_t pos = idx - prev_sum;   /* 1‑based index inside the row */

    /* ---- count the required bucket ---- */
    uint64_t low = 0;        /* tracks (i * A128) modulo SCALE */
    uint64_t count = 0;
    uint64_t n    = r;       /* interval width denominator */

    uint64_t i_max = r * r;  /* i runs from 1 to r² */

    for (uint64_t i = 1; i <= i_max; ++i) {
        low += INC;
        if (low >= MASK + 1) low -= MASK + 1;          /* wrap modulo 2^60 */

        /* bucket = floor( low * n / 2^60 ) + 1  (values 1 … n) */
        unsigned __int128 prod = (unsigned __int128)low * n;
        uint64_t bucket = (uint64_t)(prod >> 60) + 1;

        if (bucket == pos) ++count;
    }

    return (seq_t)count;
}
