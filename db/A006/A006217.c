/*
 * A006217  Number of down-up permutations of n+5 starting with 5.
 * https://oeis.org/A006217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 5; a(n) = 4*E(n+3) - 4*E(n+1) for n >= 1, where E(j) =
 *            A000111(j) are the Euler numbers, computed with the
 *            Seidel-Entringer boustrophedon triangle: T(0,0) = 1, T(i,0) = 0,
 *            T(i,j) = T(i,j-1) + T(i-1,i-j), E(i) = T(i,i)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *eul = 0;
static seq_t *rowA = 0;
static seq_t *rowB = 0;
static idx_t done = 0;
static idx_t cap = 0;

static void ensure_euler(idx_t m) {
    if (cap < m + 1) {
        idx_t newcap = m + 1;
        if (newcap < 2 * cap) newcap = 2 * cap;
        if (cap == 0) {
            eul = alloc_zero(newcap);
            rowA = alloc_zero(newcap);
            rowB = alloc_zero(newcap);
        } else {
            eul = grow_zero(eul, cap, newcap);
            rowA = grow_zero(rowA, cap, newcap);
            rowB = grow_zero(rowB, cap, newcap);
        }
        cap = newcap;
    }
    if (done == 0) {
        eul[0] = (seq_t)1;
        rowA[0] = (seq_t)1;
        done = 1;
    }
    for (idx_t i = done; i <= m; i++) {
        rowB[0] = (seq_t)0;
        for (idx_t j = 1; j <= i; j++) {
            rowB[j] = rowB[j - 1] + rowA[i - j];
        }
        eul[i] = rowB[i];
        seq_t *t = rowA;
        rowA = rowB;
        rowB = t;
    }
    if (done < m + 1) done = m + 1;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)5;
    ensure_euler(n + 3);
    seq_t d = eul[n + 3] - eul[n + 1];
    return (seq_t)4 * d;
}
