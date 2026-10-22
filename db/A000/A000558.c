/*
 * A000558  Generalized Stirling numbers of second kind.
 * https://oeis.org/A000558
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} Stirling2(n,k) * (2^(k-1) - 1) =
 *            (Sum_{k=1..n} Stirling2(n,k)*2^(k-1)) - Bell(n), since
 *            Stirling2(k,2) = 2^(k-1) - 1; rows of Stirling2 built by S(i,k)
 *            = k*S(i-1,k) + S(i-1,k-1), S(0,0) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define A558_MAX 20000

static seq_t a558_row[A558_MAX + 2];
static seq_t a558_res[A558_MAX + 2];
static idx_t a558_done = 0;
static bool a558_init = false;

seq_t a(idx_t n) {
    if (n > A558_MAX) return 0;
    if (!a558_init) {
        for (idx_t k = 0; k < A558_MAX + 2; k++) {
            a558_row[k] = 0;
            a558_res[k] = 0;
        }
        a558_row[0] = 1;
        a558_done = 0;
        a558_init = true;
    }
    while (a558_done < n) {
        idx_t i = a558_done + 1;
        for (idx_t k = i; k >= 1; k--)
            a558_row[k] = (seq_t)k * a558_row[k] + a558_row[k - 1];
        a558_row[0] = 0;
        a558_done = i;

        seq_t h = 0, bell = 0;
        for (idx_t k = i; k >= 1; k--) {
            h = h * 2 + a558_row[k];
            bell += a558_row[k];
        }
        a558_res[i] = h - bell;
    }
    return a558_res[n];
}
