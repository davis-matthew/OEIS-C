/*
 * A090925  Permutation of natural numbers arising from a square spiral.
 * https://oeis.org/A090925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the natural number located at index n when the * numbers
 *            1,2,3,… are written in a counter‑clockwise square spiral * and
 *            then read by concentric rings in the order * (r−1,r) → left
 *            along the top edge → down the left side → * right along the
 *            bottom edge → up the right side to the corner. *...
 * Range:     u8 n <= 239, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static inline unsigned long long isqrt_u64(unsigned long long x) {
    unsigned long long r = 0;
    for (int i = 63; i >= 0; --i) {
        unsigned long long bit = 1ULL << i;
        unsigned long long tmp = r + bit;
        if (tmp <= x / tmp) r = tmp;
    }
    return r;
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;

    /* Determine ring r: smallest r such that n ≤ (2r+1)² */
    unsigned long long t = isqrt_u64((unsigned long long)n - 1ULL);
    idx_t r = (t + 1ULL) / 2ULL;          /* ring index (r ≥ 1) */

    /* Index of the first element in ring r */
    idx_t first = (2ULL * r - 1ULL);
    first = first * first + 1ULL;         /* first = (2r-1)² + 1 */

    idx_t pos = n - first;                /* 0‑based position within the ring */

    long long x, y;                       /* cube coordinates of the element */

    if (pos < 2ULL * r) {                 /* top edge, leftwards */
        x = (long long)(r - 1ULL) - (long long)pos;
        y = (long long)r;
    } else if (pos < 4ULL * r) {          /* left side, downwards */
        idx_t pos2 = pos - 2ULL * r;
        x = -(long long)r;
        y = (long long)r - 1LL - (long long)pos2;
    } else if (pos < 6ULL * r) {          /* bottom edge, rightwards */
        idx_t pos2 = pos - 4ULL * r;
        x = -(long long)r + 1LL + (long long)pos2;
        y = -(long long)r;
    } else {                              /* right side upward + corner */
        idx_t rem = pos - 6ULL * r;
        if (rem < 2ULL * r - 1ULL) {      /* right side, upwards */
            x = (long long)r;
            y = -(long long)r + 1LL + (long long)rem;
        } else {                          /* the corner (r,r) */
            x = (long long)r;
            y = (long long)r;
        }
    }

    /* The natural number at coordinate (x,y) :
     * start of natural numbers for ring r is (2r-1)² + 1 */
    seq_t start = (seq_t)(2ULL * r - 1ULL);
    start = start * start + 1ULL;

    seq_t value;
    if (x == (long long)r && y > -(long long)r) {               /* right side */
        value = start + (seq_t)(y + (long long)r - 1LL);
    } else if (y == (long long)r && x < (long long)r) {         /* top edge */
        value = start + (seq_t)(2ULL * r + ((long long)r - 1LL - x));
    } else if (x == -(long long)r && y < (long long)r) {        /* left side */
        value = start + (seq_t)(4ULL * r + ((long long)r - 1LL - y));
    } else if (y == -(long long)r && x > -(long long)r) {       /* bottom edge */
        value = start + (seq_t)(6ULL * r + (x + (long long)r - 1LL));
    } else {                                                   /* corner (r,r) */
        value = start + (seq_t)(2ULL * r - 1ULL);
    }
    return value;
}
