/*
 * A221242  Number of nX4 arrays of occupancy after each element moves to some
 *          horizontal, diagonal or antidiagonal neighbor.
 * https://oeis.org/A221242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 148*a(n-1) – 6427*a(n-2) + 118138*a(n-3) * –
 *            1053200*a(n-4) + 4793575*a(n-5) * – 11306353*a(n-6) +
 *            13398822*a(n-7) * – 7161939*a(n-8) + 1435401*a(n-9) * –
 *            59049*a(n-10) * Initial values: * a(1)=4, a(2)=484, a(3)=45369,
 *            a(4)=4096576, a(5)=367565584, * a(6)=32940435025,
 *            a(7)=2951293792489, a...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"

static void init_terms(seq_t *arr) {
    static const char *s[10] = {
        "4",
        "484",
        "45369",
        "4096576",
        "367565584",
        "32940435025",
        "2951293792489",
        "264404315544196",
        "23687401278880161",
        "2122092173450945536"
    };
    for (int i = 0; i < 10; ++i)
        arr[i] = seq_from_str(s[i]);
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;

    static seq_t init[10];
    static bool init_done = false;
    if (!init_done) {
        init_terms(init);
        init_done = true;
    }

    if (n <= 10) return init[n - 1];

    /* coefficients of the linear recurrence (positive first, negative sign ignored) */
    static const int64_t coeff[10] = {
        148,  -6427, 118138, -1053200,
        4793575, -11306353, 13398822, -7161939,
        1435401, -59049
    };

    /* ring buffer holding the last 10 computed terms:
     * cur[0] = a(k-1), cur[1] = a(k-2), ..., cur[9] = a(k-10)
     * Initially set to a(10)..a(1)
     */
    seq_t cur[10];
    for (int i = 0; i < 10; ++i)
        cur[i] = init[9 - i];

    for (idx_t k = 11; k <= n; ++k) {
        seq_t pos = 0, neg = 0;
        for (int i = 0; i < 10; ++i) {
            seq_t term = cur[i];
            if (coeff[i] >= 0)
                pos += term * (seq_t)coeff[i];
            else
                neg += term * (seq_t)(-coeff[i]);
        }
        seq_t next = pos - neg;   /* guaranteed non‑negative */

        /* shift buffer: drop a(k-(current+1)), make room for next */
        for (int i = 9; i > 0; --i)
            cur[i] = cur[i - 1];
        cur[0] = next;
    }

    return cur[0];
}
