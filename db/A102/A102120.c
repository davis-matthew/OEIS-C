/*
 * A102120  Iccanobirt numbers (10 of 15): a(n) = R(a(n-1) + R(a(n-2)) +
 *          a(n-3)), where R is the digit reversal function A004086.
 * https://oeis.org/A102120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = R(a(n-1) + R(a(n-2)) + a(n-3)), with a(0)=0, a(1)=0,
 *            a(2)=1
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 30, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 32 (2026-11-03)
 * Helpers:   mem, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The digit reversal function R is provided in ../utils/rev.h. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/reverse_digits.h"
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Memoised computation of the sequence. */
seq_t a(idx_t n)
{
    /* Static storage for previously computed terms. */
    static seq_t *seq = NULL;
    static idx_t capacity = 0;     /* allocated size */
    static idx_t last = 2;         /* highest index that has been filled */

    /* Helper to grow the storage. */
    if (seq == NULL) {
        capacity = 4;
        seq = mem_resize(seq_t, NULL, capacity);
        if (!seq) return 0;          /* out‑of‑memory – unlikely in tests */ 
        /* initialise the known first three values */
        seq[0] = 0;
        seq[1] = 0;
        seq[2] = 1;
    } else if (n + 1 > capacity) {
        while (capacity <= n) capacity <<= 1;
        seq = mem_resize(seq_t, seq, capacity);
        if (!seq) return 0;
    }

    /* Direct answer for the small indices. */
    if (n <= 2) return seq[n];

    /* Extend the table up to n, computing new terms. */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t rev_prev2 = reverse_digits(seq[i - 2]);           /* R(a(i-2)) */
        seq_t sum = seq[i - 1] + rev_prev2 + seq[i - 3]; /* a(i-1)+R(a(i-2))+a(i-3) */
        seq[i] = reverse_digits(sum);                           /* a(i) = R(sum) */
    }
    last = n;
    return seq[n];
}
