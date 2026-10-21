/*
 * A055484  Number of unlabeled 3-element intersecting families (with not
 *          necessarily distinct sets) of an n-element set.
 * https://oeis.org/A055484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 99, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  a(n) satisfies the linear recurrence
 *   a(n) = -q1·a(n-1) - q2·a(n-2) - … - q14·a(n-14) + p_n
 *
 * where the coefficients come from the rational generating function
 *   A(x) = -x(x^3-x^2-1)(x^6+x^4+2x^3+x^2+1) /
 *          ( (x^3-1)^2 (x^2-1)^2 (x-1)^4 ).
 *
 * The coefficients are:
 *   q = { -4,  4,  2, -2, -4, -3, 12, -3, -4, -2,  2,  4, -4,  1 }
 *   p = { 0,  1,  0,  2,  1,  2,  1,  0, -1,  1, -1 }   // p[0] unused
 *
 * Initial values (offset 1):
 *   a(1)=1, a(2)=4, a(3)=14, a(4)=39, a(5)=96, a(6)=213, a(7)=437,
 *   a(8)=837, a(9)=1520, a(10)=2632, a(11)=4380, a(12)=7040,
 *   a(13)=10979, a(14)=16668
 *
 * The implementation below works for any idx_t n ≥ 1,
 * computing the sequence in O(n) time with only 14-term
 * recurrence and no expensive number‑theory calls.
 */

static const seq_t init[14] = {
    1, 4, 14, 39, 96, 213, 437, 837,
    1520, 2632, 4380, 7040, 10979, 16668
};

/* recurrence coefficients q[0]..q[13] corresponding to a(n-1), a(n-2), ... */
static const int q[14] = {
    -4,  4,  2, -2, -4, -3, 12, -3, -4, -2,  2,  4, -4,  1
};

/* p[1]..p[10] for the inhomogeneous part; p[0] unused, p[>10] = 0 */
static const int p[11] = {
    0,  /* p[0] unused */
    1, 0, 2, 1, 2, 1, 0, -1, 1, -1
};

seq_t a(idx_t n) {
    /* Return known value for n ≤ 14 */
    if (n <= 14) {
        return init[n - 1];
    }

    /* Hold the last 14 computed values: last[i] = a(n-1-i) */
    seq_t last[14];
    for (int i = 0; i < 14; ++i) {
        last[i] = init[i];
    }

    seq_t result = 0;          /* result for the current n */
    for (idx_t cur = 15; cur <= n; ++cur) {
        seq_t sum_pos = 0;     /* sum of q_k * a(cur-k) for q_k > 0 */
        seq_t sum_neg = 0;     /* sum of (-q_k) * a(cur-k) for q_k < 0 */

        /* Accumulate sums using the last 14 values */
        for (int k = 0; k < 14; ++k) {
            seq_t val = last[13 - k];   /* a(cur-1-k) */
            int coeff = q[k];
            if (coeff > 0) {
                sum_pos += (seq_t)coeff * val;
            } else {
                sum_neg += (seq_t)(-coeff) * val;
            }
        }

        /* Compute a(cur) = sum_neg - sum_pos + p[cur] */
        seq_t a_cur = sum_neg - sum_pos;
        int p_val = (cur <= 10) ? p[cur] : 0;
        if (p_val > 0) {
            a_cur += (seq_t)p_val;
        } else if (p_val < 0) {
            a_cur -= (seq_t)(-p_val);
        }

        /* Shift the circular buffer and store the new value */
        for (int i = 0; i < 13; ++i) {
            last[i] = last[i + 1];
        }
        last[13] = a_cur;
        result = a_cur;
    }

    return result;
}
