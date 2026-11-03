/*
 * A096465  Triangle (read by rows) formed by setting all entries in the first
 *          column and in the main diagonal ((i,i) entries) to 1 and the rest
 *          of the entries by the recursion T(n, k) = T(n-1, k) + T(n, k-1).
 * https://oeis.org/A096465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth element of the triangle defined by T(0,0)=1,
 *            T(n,0)=1, T(n,n)=1 and the recurrence T(n,k)=T(n-1,k)+T(n,k-1).
 *            For a given index n, let r = floor((sqrt(8n+1)-1)/2) and k = n -
 *            r*(r+1)/2; then a(n) = T(r,k)
 * Range:     u8 n <= 40, u16 n <= 87, u32 n <= 248, u64 n <= 774,
 *            u128 n <= 2552, big any n
 * Validated: standard, pass, 94 terms through n = 8174 (2026-11-03)
 * Helpers:   mem, triangular
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/triangular.h"
#include "../utils/mem.h"

/* find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
static inline idx_t find_row(idx_t n) {
    idx_t low = 0, high = n + 1;
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (triangular(mid) <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n) {
    /* static cache of the triangle up to the largest row seen */
    static seq_t *data = NULL;
    static idx_t data_len = 0;
    static idx_t last_row = (idx_t)-1;

    idx_t r = find_row(n);
    idx_t needed_len = (unsigned __int128)(r + 1) * (r + 2) / 2;

    if (data_len < needed_len) {
        seq_t *new_data = mem_resize(seq_t, data, (size_t)needed_len);
        if (!new_data) exit(EXIT_FAILURE);
        data = new_data;

        /* build triangle rows 0..r */
        for (idx_t rr = 0; rr <= r; ++rr) {
            idx_t base = triangular(rr);           /* offset of T(rr,0) */
            data[base] = (seq_t)1;
            for (idx_t kk = 1; kk < rr; ++kk) {
                seq_t top   = data[triangular(rr - 1) + kk];
                seq_t left  = data[base + kk - 1];
                data[base + kk] = top + left;
            }
            data[base + rr] = (seq_t)1;           /* T(rr,rr) */
        }
        data_len = needed_len;
        last_row = r;
    }

    idx_t start   = triangular(r);
    idx_t offset  = start + (n - start);
    return data[offset];
}
