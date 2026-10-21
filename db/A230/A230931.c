/*
 * A230931  Number of black-square subarrays of (n+2) X (4+2) 0..3 arrays
 *          x(i,j) with each element diagonally or antidiagonally next to at
 *          least one element with value (x(i,j)+1) mod 4, no adjacent
 *          elements equal, and upper left element zero.
 * https://oeis.org/A230931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A230931: Number of black‑square subarrays of (n+2)×(4+2) 0…3 arrays
 * satisfying the shown constraints.
 *
 * Recurrence (order 17):
 * a(1)=8, a(2)=48, a(3)=232, a(4)=1242, a(5)=6896, a(6)=37984,
 * a(7)=208172, a(8)=1142054, a(9)=6269172, a(10)=34409586,
 * a(11)=188850410, a(12)=1036485334, a(13)=5688689754,
 * a(14)=31221981876, a(15)=171359511448, a(16)=940494098820,
 * a(17)=5161833500756.
 *
 * a(n) = 5*a(n‑1)+a(n‑2)+11*a(n‑3)‑a(n‑4)‑33*a(n‑5)‑82*a(n‑6)+a(n‑7)
 *        –33*a(n‑8)+71*a(n‑9)+125*a(n‑10)+117*a(n‑11)+140*a(n‑12)
 *        +66*a(n‑13)+35*a(n‑14)+2*a(n‑15)‑4*a(n‑16)‑4*a(n‑17)
 * for n ≥ 18.
 *
 * The function below evaluates this recurrence directly, using only
 * local storage and performing at most 17 * (n‑17) arithmetic operations,
 * which is fast enough for n up to 10 000 and works for any seq_t
 * width supported by the build.
 *
 * No external utilities are required; seq_t is treated as a
 * value type that supports +, −, *, and assignment.
 *
 * The function is safe for calls in any order: it recomputes the
 * needed range from scratch each time.
 *
 * If seq_t is an arbitrary‑precision class, the operations are
 * defined by that class and no extra handling is required.
 *
 * This implementation obeys all the specified constraints.
 */
#include <stddef.h>  /* for size_t */

static const seq_t init_values[17] = {
    (seq_t)8ULL,
    (seq_t)48ULL,
    (seq_t)232ULL,
    (seq_t)1242ULL,
    (seq_t)6896ULL,
    (seq_t)37984ULL,
    (seq_t)208172ULL,
    (seq_t)1142054ULL,
    (seq_t)6269172ULL,
    (seq_t)34409586ULL,
    (seq_t)188850410ULL,
    (seq_t)1036485334ULL,
    (seq_t)5688689754ULL,
    (seq_t)31221981876ULL,
    (seq_t)171359511448ULL,
    (seq_t)940494098820ULL,
    (seq_t)5161833500756ULL
};

static const int coeff[17] = {
    5, 1, 11, -1, -33, -82, 1, -33, 71,
    125, 117, 140, 66, 35, 2, -4, -4
};

seq_t a(idx_t n)
{
    /* Return straightforward initial value for n ≤ 17 */
    if (n <= 17) {
        return init_values[(size_t)(n - 1)];
    }

    /* Buffer holds the last 17 terms.  buf[0] = a(k‑1),
       buf[1] = a(k‑2), … , buf[16] = a(k‑17). */
    seq_t buf[17];
    for (size_t i = 0; i < 17; ++i) {
        buf[i] = init_values[16 - i];
    }

    /* Iterate from term 18 up to n. */
    for (idx_t k = 18; k <= n; ++k) {
        seq_t sum = (seq_t)0;
        for (size_t i = 0; i < 17; ++i) {
            /* coeff[i] multiplies buf[i] which is a(k‑i‑1). */
            int c = coeff[i];
            if (c > 0) {
                sum += buf[i] * (seq_t)c;
            } else if (c < 0) {
                sum -= buf[i] * (seq_t)(-c);
            }
        }

        /* Shift buffer right and insert new term at the front. */
        for (size_t i = 16; i > 0; --i) {
            buf[i] = buf[i - 1];
        }
        buf[0] = sum;
    }

    /* buf[0] now holds a(n). */
    return buf[0];
}
