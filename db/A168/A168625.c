/*
 * A168625  Triangle T(n,k) = 8*binomial(n,k) - 7 with columns 0 <= k <= n.
 * https://oeis.org/A168625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the n‑th entry of the flattened triangle defined by
 *            T(r,k) = 8 * binomial(r,k) - 7
 * Range:     u8 n <= 30, u16 n <= 142, u32 n <= 542, u64 n <= 2173,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* --------------------------------------------------------------------
   Determine the row index r such that r(r+1)/2 <= n < (r+1)(r+2)/2
   (i.e. the index n lies in row r in the triangular arrangement).
   -------------------------------------------------------------------- */
static inline idx_t row_from_index(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 1;

    /* Exponential search to find an upper bound. */
    while ((unsigned __int128)high * (high + 1) / 2 <= n)
        high <<= 1;

    /* Binary search for the exact row. */
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* --------------------------------------------------------------------
   Compute binomial(n,k) safely.  For 64‑bit integers we use __int128
   to avoid intermediate overflow; for arbitrary‑precision seq_t we
   rely on the operator overloads of that type.
   -------------------------------------------------------------------- */
static inline seq_t binom_safe(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        idx_t num = n - k + i;

        if (sizeof(seq_t) <= sizeof(unsigned long long)) {
            unsigned __int128 tmp = (unsigned __int128)res * (unsigned __int128)num;
            res = (seq_t)(tmp / i);
        } else /* arbitrary‑precision build */
            res = (res * (seq_t)num) / (seq_t)i;
    }
    return res;
}

/* --------------------------------------------------------------------
   Main sequence function.
   -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Find row r and column k corresponding to linear index n. */
    idx_t r   = row_from_index(n);
    idx_t prev = (idx_t)(((unsigned __int128)r * (r + 1)) / 2);
    idx_t k   = n - prev;

    /* T(r,k) = 8 * binomial(r,k) - 7. */
    seq_t bin = binom_safe(r, k);
    seq_t val;
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        unsigned __int128 tmp = (unsigned __int128)bin * 8;
        val = (seq_t)(tmp - 7);
    } else /* arbitrary‑precision build */
        val = bin * 8 - 7;

    return val;
}
