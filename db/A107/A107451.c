/*
 * A107451  Let m = 5 and set M = {{0, 1, 0, 0, 0, 0}, {0, 0, 1, 0, 0, 0}, {0,
 *          0, 0, 1, 0, 0}, {0, 0, 0, 0, 1, 0}, {0, 0, 0, 0, 0, 1}, {-1, m, (m
 *          + 1), -m*(m + 1), -m, (m + 2)}}. Let v[0] = {0, 1, 1, 2, 3, 5},
 *          v[n] = M.v[n - 1]. Then a = Abs[v[n][[1]].
 * https://oeis.org/A107451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sequence a(n) defined by the linear recurrence v[n] = M * v[n-1]
 *            with m = 5, v[0] = {0,1,1,2,3,5}, M =
 *            {{0,1,0,0,0,0},{0,0,1,0,0,0},{0,0,0,1,0,0},
 *            {0,0,0,0,1,0},{0,0,0,0,0,1}, {-1,5,6,-30,-5,7}}, a(n) =
 *            |v[n][1]|
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper structure for signed magnitude arithmetic. */
typedef struct { seq_t mag; bool neg; } sb;

/* Return zero signed magnitude. */
static inline sb sb_zero(void) { return (sb){0, false}; }

/* Return signed magnitude from an unsigned value. */
static inline sb sb_from(seq_t v) { return (sb){v, false}; }

/* Addition of two signed magnitudes. */
static inline sb sb_add(sb a, sb b)
{
    if (a.neg == b.neg) {
        return (sb){a.mag + b.mag, a.neg};
    } else {
        if (a.mag > b.mag) {
            return (sb){a.mag - b.mag, a.neg};
        } else if (b.mag > a.mag) {
            return (sb){b.mag - a.mag, b.neg};
        } else {
            return sb_zero();
        }
    }
}

/* Sequence implementation. */
seq_t a(idx_t n)
{
    sb v[6];
    v[0] = sb_from(0);
    v[1] = sb_from(1);
    v[2] = sb_from(1);
    v[3] = sb_from(2);
    v[4] = sb_from(3);
    v[5] = sb_from(5);

    static const int coeff[6] = {-1, 5, 6, -30, -5, 7};

    for (idx_t step = 1; step <= n; ++step) {
        sb newV[6];
        newV[0] = v[1];
        newV[1] = v[2];
        newV[2] = v[3];
        newV[3] = v[4];
        newV[4] = v[5];

        sb sum = sb_zero();
        for (int i = 0; i < 6; ++i) {
            if (coeff[i] == 0) continue;
            sb term;
            seq_t absc = coeff[i] > 0 ? (seq_t)coeff[i] : (seq_t)(-coeff[i]);
            term.mag = absc * v[i].mag;
            term.neg = v[i].neg ^ (coeff[i] < 0);
            sum = sb_add(sum, term);
        }
        newV[5] = sum;

        for (int i = 0; i < 6; ++i) v[i] = newV[i];
    }

    /* a(n) = |v[n][1]|, magnitude already stored. */
    return v[1].mag;
}
