/*
 * A162527  Numbers k whose largest divisor <= sqrt(k) equals 7.
 * https://oeis.org/A162527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 1165, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A162527: Numbers k whose largest divisor <= sqrt(k) equals 7.
   Computed by brute‑force test of successive multiples of 7: keep k for which
   there is no divisor d with 8 <= d <= floor(sqrt(k)).  a(n) (n>=1) is the
   n-th such k. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t len = 0, cap = 0;
    static seq_t next = (seq_t)49;   /* first candidate (7*7) */

    while (len < n) {
        seq_t k = next;
        int ok = 1;
        for (seq_t d = 8; d * d <= k; ++d) {
            if (k % d == 0) {
                ok = 0;
                break;
            }
        }
        if (ok) {
            if (len == cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                cap = new_cap;
            }
            seq[len++] = k;
        }
        next += 7;   /* next multiple of 7 */
    }

    return seq[n - 1];
}
