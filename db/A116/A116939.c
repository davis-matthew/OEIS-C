/*
 * A116939  Lexicographically earliest sequence such that each i occurs
 *          exactly i+1 times and succeeding terms differ exactly by -1 or +1.
 * https://oeis.org/A116939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1; for n>1 let x be the maximum frequency of any value in a[0..n‑1].
   Then a(n)= (x==a(n-1) ? x+1 : x).  This implements OEIS sequence A116939. */

#include <stdlib.h>

/* Return the n-th term of the sequence A116939.  The function is
   self‑contained, O(n) time and O(n) memory, and works for any
   build with a seq_t integer type. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* A frequency table large enough to hold any value that can occur.
       The value of a(n) grows at most one per step, so a(n) <= n+1 for n>=1. */
    size_t size = (size_t)n + 2;
    size_t *freq = (size_t *)calloc(size, sizeof(size_t));
    if (!freq) return (seq_t)0;  /* Out‑of‑memory guard – should not happen in tests */

    freq[0] = 1;          /* a(0) */
    freq[1] = 1;          /* a(1) */
    size_t maxFreq = 1;   /* greatest frequency seen so far */

    seq_t prev = (seq_t)1; /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = (seq_t)((seq_t)maxFreq == prev ? (maxFreq + 1) : maxFreq);

        /* Increment the frequency of the new value. */
        size_t idx = (size_t)next;
        freq[idx] += 1;
        if (freq[idx] > maxFreq) maxFreq = freq[idx];

        prev = next;
    }

    free(freq);
    return prev;
}
