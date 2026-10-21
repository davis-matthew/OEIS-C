/*
 * A057027  Triangle T read by rows: row n consists of the numbers C(n,2)+1 to
 *          C(n+1,2); numbers in odd-numbered places form an increasing
 *          sequence and the others a decreasing sequence.
 * https://oeis.org/A057027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = value in the triangular array where row r consists of the
 *            integers C(r,2)+1 .. C(r+1,2). In each row, odd-indexed
 *            positions are filled with the smallest remaining number,
 *            even-indexed positions with the largest remaining number. This
 *            yields the closed‑form: if j odd -> T = L + (j-1)/2 if j...
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the sequence value for index n (1‑based). */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; behaviour for n=0 is unused */
        return 0;

    /* --- Find the row r such that r(r+1)/2 >= n --- */
    uint64_t lo = 1, hi = n;          /* r never exceeds n  */
    while (lo < hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        /* use 128‑bit arithmetic to avoid overflow */
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    uint64_t r = lo;

    /* --- Column index within the row (1‑based) --- */
    unsigned __int128 prev_tri = (unsigned __int128)r * (r - 1) / 2;
    uint64_t j = (uint64_t)(n - prev_tri);   /* 1 <= j <= r */

    /* --- Compute the bounds of the row --- */
    seq_t L = (seq_t)r * ((seq_t)r - 1) / 2 + 1;   /* C(r,2)+1  */
    seq_t R = (seq_t)r * ((seq_t)r + 1) / 2;       /* C(r+1,2)  */

    /* --- Evaluate the position --- */
    if (j & 1) {                     /* odd position: increasing part */
        return L + (seq_t)((j - 1) / 2);
    } else {                         /* even position: decreasing part */
        return R - (seq_t)(j / 2) + 1;
    }
}
