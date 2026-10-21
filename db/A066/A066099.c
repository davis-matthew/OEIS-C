/*
 * A066099  Triangle read by rows, in which row n lists the compositions of n
 *          in reverse lexicographic order.
 * https://oeis.org/A066099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn, tabf
 * Formula:   a(n) = A030303(n+1) - A030303(n)
 * Range:     u8 n <= 268, u16 n <= 5120, u32 n <= 5120, u64 n <= 5120,
 *            u128 n <= 5120, big any n
 * Helpers:   popcount_sum_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/popcount_sum_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used, but safe */

    /* ---------- find the row r that contains index n ----------------- */
    seq_t cum = 0;                  /* entries in previous rows   */
    idx_t r = 0;                    /* current row number (1‑based) */
    while (1) {
        ++r;
        seq_t two_pow = ((seq_t)1) << (r - 1);           /* 2^(r-1)   */
        seq_t T = two_pow * ((seq_t)(r + 1)) / 2;        /* row length */
        if (cum + T > (seq_t)(n - 1))
            break;
        cum += T;
    }

    /* offset (0‑based) inside the found row */
    idx_t offset = (idx_t)((seq_t)n - cum - 1);

    /* ---------- binary search for the composition id ----------------- */
    idx_t max_id = (((idx_t)1) << (r - 1)) - 1;           /* 2^(r-1)-1  */
    idx_t low = 0, high = max_id;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        seq_t s_mid = ((seq_t)(mid + 1)) + popcount_sum_u64(mid);
        if (s_mid > offset)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t comp = low;  /* composition binary id */

    /* number of parts in all preceding compositions of this row */
    seq_t before = 0;
    if (comp > 0)
        before = ((seq_t)comp) + popcount_sum_u64(comp - 1);
    idx_t part_idx = offset - (idx_t)before;   /* 0‑based part index */

    /* ---------- find the requested part value ----------------------- */
    int part_len = 1;           /* length of current part          */
    idx_t cur_part = 0;         /* index of current part           */

    for (idx_t p = 1; p < r; ++p) {
        idx_t bit_pos = r - p - 1;                 /* map to bit index */
        if (comp & ((idx_t)1 << bit_pos)) {        /* cut after position p */
            if (cur_part == part_idx)
                return (seq_t)part_len;            /* return found part  */
            ++cur_part;
            part_len = 1;                          /* start next part   */
        } else {
            ++part_len;
        }
    }
    /* last part of the composition */
    if (cur_part == part_idx)
        return (seq_t)part_len;
    return (seq_t)part_len;
}
