/*
 * A164144  Number of binary strings of length n with equal numbers of 001 and
 *          101 substrings.
 * https://oeis.org/A164144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) equals the number of binary strings of length n * having
 *            the same number of 001 and 101 substrings. * * Computation:
 *            dynamic programming over the last two bits and the * current
 *            difference Δ = (#001) – (#101). State indices are * (last two
 *            bits) × (Δ offset). Transitions add a new bit; the *...
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 34, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Small n – no 3‑bit substrings exist. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;

    /* Range for the difference Δ is [-(n-2), +(n-2)]. */
    size_t N     = (size_t)n;
    size_t range = 2 * N + 5;          /* safe buffer on both sides */
    size_t total = 4 * range;          /* 4 states × all Δ values */

    /* Allocate two DP layers.  Use new[] if compiling as C++,
     * otherwise malloc.  The arrays contain only POD values
     * for integer types, and default‐constructed values for
     * arbitrary‑precision types. */
#ifdef __cplusplus
    seq_t *cur = new seq_t[total];
    seq_t *nxt = new seq_t[total];
#else
    seq_t *cur = (seq_t *)malloc(total * sizeof(seq_t));
    seq_t *nxt = (seq_t *)malloc(total * sizeof(seq_t));
#endif
    if (!cur || !nxt) return (seq_t)0;          /* allocation failed */

    /* Clear arrays. */
    memset(cur, 0, total * sizeof(seq_t));
    memset(nxt, 0, total * sizeof(seq_t));

    /* Use a byte offset to convert a Δ value to an array index: */
    const size_t offset = N;                    /* safe offset */

    /* Initialise the DP for strings of length 2.
     * Every state (last two bits) can be reached by exactly one string,
     * and Δ = 0 for all of them. */
    for (int s = 0; s < 4; ++s)
        cur[s * range + offset] = (seq_t)1;

    /* Extend the strings one bit at a time from length 3 up to n. */
    for (idx_t len = 3; len <= n; ++len)
    {
        int prev_min = -(int)(len - 3);   /* Δ range for previous length */
        int prev_max =  (int)(len - 3);
        int new_min  = -(int)(len - 2);   /* Δ range for current length  */
        int new_max  =  (int)(len - 2);

        /* Clear the next layer. */
        memset(nxt, 0, total * sizeof(seq_t));

        /* Iterate over all 4 states (a,b) of the last two bits. */
        for (int s = 0; s < 4; ++s)
        {
            int a_bit = (s >> 1) & 1;  /* older of the two bits */
            int b_bit =  s        & 1;

            /* New states after appending a bit. */
            int ns0 = (b_bit << 1);          /* (b,0)  */
            int ns1 = (b_bit << 1) | 1;      /* (b,1)  */

            /* Δ change when the new bit is 1. */
            int delta1 = 0;
            if (a_bit == 0 && b_bit == 0) delta1 =  1;
            else if (a_bit == 1 && b_bit == 0) delta1 = -1;

            seq_t *cur_base = cur + s * range;
            seq_t *nxt0     = nxt + ns0 * range;
            seq_t *nxt1     = nxt + ns1 * range;

            /* Process all Δ values that can currently occur. */
            for (int d = prev_min; d <= prev_max; ++d)
            {
                seq_t val = cur_base[offset + d];
                if (!val) continue;

                /* Append 0 – Δ unchanged. */
                nxt0[offset + d] += val;

                /* Append 1 – Δ changes by delta1. */
                nxt1[offset + d + delta1] += val;
            }
        }

        /* Prepare for next iteration.  Swap the current and next layers. */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* Sum all states with Δ = 0 to obtain the answer. */
    seq_t result = 0;
    for (int s = 0; s < 4; ++s)
        result += cur[s * range + offset];

    /* Free the allocated memory. */
#ifdef __cplusplus
    delete [] cur;
    delete [] nxt;
#else
    free(cur);
    free(nxt);
#endif

    return result;
}
