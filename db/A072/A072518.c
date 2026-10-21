/*
 * A072518  Index of smallest triangular number beginning with the n-th
 *          triangular number other than itself.
 * https://oeis.org/A072518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = min{ m>n | T_m starts with T_n and T_m ≠ T_n } where T_k
 *            = k(k+1)/2 is the k‑th triangular number
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   seq_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_len.h"

seq_t a(idx_t n) {
    /* pre‑compute powers of 10 once */
    static bool initialized = false;
    static seq_t pow10[60];
    if (!initialized) {
        pow10[0] = 1;
        for (int i = 1; i < 60; ++i)
            pow10[i] = pow10[i-1] * 10;
        initialized = true;
    }

    /* triangular number for index n */
    seq_t T_n = ((seq_t)n * ((seq_t)n + 1)) / 2;
    int len_n = seq_len(T_n);

    /* start searching from n+1 */
    seq_t T_cur = T_n;          /* T_{m-1} initially */
    for (idx_t m = n + 1;; ++m) {
        T_cur += m;             /* now T_m = T_{m-1} + m */
        seq_t T_m = T_cur;
        int len_m = seq_len(T_m);

        if (len_m < len_n) continue;           /* unreachable, but keep for safety */

        seq_t prefix;
        if (len_m == len_n) {
            prefix = T_m;
        } else {
            int shift = len_m - len_n;          /* number of digits to drop */
            prefix = T_m / pow10[shift];
        }

        if (prefix == T_n)
            return (seq_t)m;
    }
}
