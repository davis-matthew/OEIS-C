/*
 * A169671  Lexicographically earliest de Bruijn sequence for n = 6 and k = 2.
 * https://oeis.org/A169671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1023, u16 n <= 1023, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   db
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Lexicographically earliest binary de Bruijn sequence of order 6 (period 64) */
#include <stdbool.h>
#include "../utils/db.h"

/* External function required by the build system */
seq_t a(idx_t n) {
    static bool init = false;
    static seq_t period[64];
    if (!init) {
        const int k = 2;
        const int norder = 6;
        int a_arr[(2 * 6) + 1];
        a_arr[0] = 0;
        int seq_arr[64];
        int seq_len = 0;
        db(1, 1, norder, k, a_arr, seq_arr, &seq_len);
        for (int i = 0; i < 64; ++i) {
            period[i] = (seq_t)seq_arr[i];
        }
        init = true;
    }
    return period[n % 64];
}
