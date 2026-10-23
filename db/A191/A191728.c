/*
 * A191728  Dispersion of A047209, (numbers >1 and congruent to 1 or 4 mod 5),
 *          by antidiagonals.
 * https://oeis.org/A191728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the n‑th entry read from the dispersion of A047209 *
 *            (numbers >1 congruent to 1 or 4 mod 5) by antidiagonals. * Each
 *            row r (0‑based) starts with r‑th integer not belonging to that
 *            set; * each subsequent column is obtained by iterating * f(x) =
 *            ⌊5·x/2⌋ + (x odd? 2 : 1) (x > 1 or =1). * The...
 * Range:     u8 n <= 21, u16 n <= 78, u32 n <= 300, u64 n <= 1176,
 *            u128 n <= 4753, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return the n-th term of A191728 (n ≥ 1). */
seq_t a(idx_t n) {
    /* Determine the diagonal d (0‑based) such that
     * the number of terms up to and including diagonal d
     * is d+1 choose 2 = (d+1)(d+2)/2 ≥ n. */
    idx_t d = 0;
    while ((__int128)(d + 1) * (d + 2) / 2 < n)
        ++d;

    /* Number of terms before diagonal d. */
    idx_t prev_diag = (__int128)d * (d + 1) / 2;

    /* Position on diagonal d (0‑based). */
    idx_t pos = n - prev_diag - 1;      /* 0 ≤ pos ≤ d */

    /* Row index r and column index c on this diagonal. */
    idx_t r = pos;
    idx_t c = d - pos;

    /* Find the r‑th starting value that is not in A047209:
     *   1, or n > 1 with n mod 5 ≠ 1 and n mod 5 ≠ 4. */
    idx_t count = 0;
    uint64_t cur = 1;          /* use 64‑bit to avoid overflow in small seq_t */
    seq_t start_val = 0;
    while (1) {
        bool allowed;
        if (cur == 1)
            allowed = true;
        else {
            uint64_t mod = cur % 5;
            allowed = (mod != 1 && mod != 4);
        }
        if (allowed) {
            if (count == r) {
                start_val = (seq_t)cur;
                break;
            }
            ++count;
        }
        ++cur;
    }

    /* Apply f repeatedly c times. */
    seq_t x = start_val;
    for (idx_t t = 0; t < c; ++t) {
        seq_t res = (x * 5) / 2;      /* floor(5*x/2) */
        if (x & 1)                    /* x odd */
            res += 2;
        else
            res += 1;
        x = res;
    }

    return x;
}
