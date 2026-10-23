/*
 * A121909  S-D transform of factorial numbers A000142.
 * https://oeis.org/A121909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A121909 (S‑D transform of factorial numbers)
   a(n) = Σ_{k=0..n} A051159(n,k) * k!
   Computed by iterating
      r_{t+1}(i) = r_t(i+1) + (-1)^i * r_t(i)
   where r_0(i) = i!   (i ≥ 0).  The first element of r_n is a(n). */
#include <stdlib.h>

/* External linkage */
seq_t a(idx_t n)
{
    /* 0! is needed; factorials up to (n+1)! are required for step n. */
    idx_t max_idx = n + 1;               /* largest factorial index used  */
    size_t array_len = (size_t)(max_idx + 1);   /* indices 0 .. n+1  */

    /* Allocate two working rows */
    seq_t *cur = (seq_t *)malloc(array_len * sizeof(seq_t));
    seq_t *nxt = (seq_t *)malloc(array_len * sizeof(seq_t));

    /* Compute factorials */
    cur[0] = (seq_t)1;
    for (idx_t i = 1; i <= max_idx; ++i)
        cur[(size_t)i] = cur[(size_t)(i - 1)] * (seq_t)i;

    /* Current row length (number of valid entries) */
    size_t cur_sz = array_len;

    /* Perform the transform n times */
    for (idx_t step = 1; step <= n; ++step) {
        /* Each next row is one element shorter */
        for (size_t i = 0; i < cur_sz - 1; ++i) {
            seq_t val = cur[i + 1];
            if ((i & 1) == 0)   /* (-1)^i == +1 for even i */
                val += cur[i];
            else
                val -= cur[i];
            nxt[i] = val;
        }
        cur_sz--;               /* next row has one fewer entry  */
        /* Swap current and next rows for next iteration */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* Result is the first element of the final row */
    seq_t result = cur[0];

    /* Clean up */
    free(cur);
    free(nxt);

    return result;
}
