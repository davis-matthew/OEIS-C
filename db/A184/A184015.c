/*
 * A184015  n-1+ceiling(n^2/e); complement of A184014.
 * https://oeis.org/A184015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n^2 / e) (since ceil(n^2/e) = floor(n^2/e)+1)
 * Range:     u8 n <= 25, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* 10^18, 18 decimal digits are enough for n up to about 1e9.
   Q = 10^18, P = ceil(e*10^18) = 2718281828459045236. */
#define Q_CONST 1000000000000000000ULL
#define P_CONST 2718281828459045236ULL

seq_t a(idx_t n) {
    /* offset 1 : a(1) is the first term. */
    if (n == 0) return 0;          /* undefined for n<1, protect anyway */

    seq_t n_seq = (seq_t)n;

    /* Handle 64‑bit integer types with 128‑bit intermediates. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 n128 = (unsigned __int128)n_seq;
        unsigned __int128 n_sq = n128 * n128;                /* n^2 */

        unsigned __int128 numer = n_sq * (unsigned __int128)Q_CONST;
        unsigned __int128 low = numer / (unsigned __int128)P_CONST;  /* floor(n^2 / e_high) */

        seq_t floor_val = (seq_t)low;                         /* floor candidate */

        /* Test whether floor_val+1 is still ≤ floor(n^2 / e). */
        unsigned __int128 test = ((unsigned __int128)floor_val + 1u) * (unsigned __int128)P_CONST;
        if (test <= numer) {
            floor_val += 1;
        }

        return n_seq + floor_val;
    }

    /* Arbitrary‑precision integers are handled directly. */
    const seq_t Q = (seq_t)Q_CONST;
    const seq_t P = (seq_t)P_CONST;

    seq_t n_sq = n_seq * n_seq;                /* n^2 */
    seq_t numer = n_sq * Q;                    /* n^2 * Q */
    seq_t low = numer / P;                     /* floor(n^2 / e_high) */
    seq_t floor_val = low;

    /* Test whether low+1 is still valid. */
    seq_t test = (floor_val + (seq_t)1) * P;
    if (test <= numer) {
        floor_val = floor_val + (seq_t)1;
    }

    return n_seq + floor_val;
}
