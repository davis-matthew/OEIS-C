/*
 * A059399  Triangular hopscotch.
 * https://oeis.org/A059399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the element of the triangular hopscotch sequence,
 *            obtained by reading the nth row of consecutive integers
 *            alternately from right and left ends
 * Range:     u8 n <= 253, u16 n <= 9724, u32 n <= 9724, u64 n <= 9724,
 *            u128 n <= 9724, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    // Find the row r such that T(r-1) < n <= T(r), where T(k)=k(k+1)/2
    double d = sqrt((double)n * 8.0 + 1.0);
    idx_t r = (idx_t)((d - 1.0) / 2.0);           // floor estimate
    // Ensure r is the correct row
    unsigned __int128 tri_r = (unsigned __int128)r * (r + 1) / 2;
    if (tri_r < n) {
        r++;
        tri_r = (unsigned __int128)r * (r + 1) / 2;
    }

    // T(r-1) = (r-1)*r/2
    unsigned __int128 t_prev = (unsigned __int128)(r - 1) * r / 2;
    unsigned __int128 start = t_prev + 1;                      // first number in row r
    unsigned __int128 pos = (unsigned __int128)n - t_prev - 1; // 0‑based position in row

    unsigned __int128 idx_offset;
    if ((pos & 1ULL) == 0) {             // even position: pick from right side
        idx_offset = (unsigned __int128)(r - 1) - (pos >> 1);
    } else {                              // odd position: pick from left side
        idx_offset = pos >> 1;
    }

    unsigned __int128 value = start + idx_offset;
    return (seq_t)((uint64_t)value);    // fits in 64 bits for all valid inputs
}
