/*
 * A076136  Numbers n such that Omega(n) = Omega(n-1) + Omega(n-2), where
 *          Omega(n) (A001222) denotes the number of prime factors of n,
 *          counting multiplicity.
 * https://oeis.org/A076136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A076136:
 * Numbers n for which Ω(n) = Ω(n-1) + Ω(n-2).
 * Ω is the total number of prime factors counted with multiplicity
 * (OEIS A001222).  
 * The implementation computes the sequence by incremental search.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/bigomega.h"

/* External linkage, as required */
seq_t a(idx_t n)
{
    /* If called with n==0, return 0 (offset starts at 1) */
    if (n == 0) {
        return 0;
    }

    /* Static globals to keep state between calls */
    static seq_t *seq   = NULL;          /* array of the terms found so far */
    static size_t  seq_len = 0;          /* number of elements stored */
    static size_t  seq_cap = 0;          /* allocated capacity of seq */

    static idx_t   checked_n = 2;        /* last number examined */
    static seq_t   omega_nm1 = 1;        /* Ω(checked_n)   (Ω(2)) */
    static seq_t   omega_nm2 = 0;        /* Ω(checked_n-1) (Ω(1)) */

    /* Grow the sequence array as needed */
    if (seq_cap == 0) {
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them */
    while (seq_len < n) {
        /* Next number to test */
        ++checked_n;
        seq_t omega_curr = bigomega(checked_n);

        /* Check the condition Ω(n) = Ω(n-1)+Ω(n-2) */
        if (omega_curr == omega_nm1 + omega_nm2) {
            if (seq_len == seq_cap) {
                size_t newcap = seq_cap * 2;
                seq = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
                seq_cap = newcap;
            }
            seq[seq_len++] = checked_n;
        }

        /* Shift Ω(n-2), Ω(n-1) for the next iteration */
        omega_nm2 = omega_nm1;
        omega_nm1 = omega_curr;
    }

    return seq[n - 1];
}
