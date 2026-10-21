/*
 * A226386  Numbers n such that rad(phi(n)) < phi(rad(n)), where rad(n) is the
 *          squarefree kernel of n, and phi is Euler's totient function.
 * https://oeis.org/A226386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 158, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A226386: returns the nth positive integer n such that rad(phi(n)) < phi(rad(n)).
   The function enumerates integers in increasing order and records those that
   satisfy the inequality. */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/rad.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;
    static idx_t next = 2;          /* next candidate to test */

    /* generate terms until we have at least n of them */
    while (seq_len < n) {
        seq_t phi_val   = calc_phi((seq_t)next);
        idx_t  phi_idx  = (idx_t)phi_val;          /* phi_val <= next <= UINT64_MAX */
        seq_t rad_phi   = rad(phi_idx);

        seq_t rad_n     = rad(next);
        seq_t phi_rad   = calc_phi(rad_n);

        if (rad_phi < phi_rad) {
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;           /* allocation failure */
                seq = tmp;
                seq_cap = new_cap;
            }
            seq[seq_len++] = (seq_t)next;
        }
        ++next;
    }
    return seq[n - 1];
}
