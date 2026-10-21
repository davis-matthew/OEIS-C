/*
 * A057033  Let P(n) of a sequence s(1),s(2),s(3),... be obtained by leaving
 *          s(1),...s(n-1) fixed and reverse-cyclically permuting every n
 *          consecutive terms thereafter; apply P(2) to 1,2,3,... to get
 *          PS(2), then apply P(3) to PS(2) to get PS(3), then apply P(4) to
 *          PS(3), etc. The limit of PS(n) is A057033.
 * https://oeis.org/A057033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes A057033.  
    The algorithm applies the permutations P(k) for k = 2..n to the
    initial sequence 1,2,3,…, keeping only the first 2n–1 terms.
    After all k ≤ n have been processed, the value at position n is
    a(n).  The sequence is generated exactly as defined in the OEIS
    entry, and the implementation works for any seq_t and idx_t
    supplied by the build.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 1)          /* a(1) = 1 */
        return 1;

    /* number of terms needed to capture all swaps affecting position n */
    size_t M = (size_t)(2 * (uint64_t)n - 1);

    /* work array holding the current permutation of 1..M */
    seq_t *arr = (seq_t *)malloc(M * sizeof(seq_t));
    if (!arr)
        return 0;        /* allocation failed – unlikely for n ≤ 10000 */

    /* initialise with the natural numbers */
    for (size_t i = 0; i < M; ++i)
        arr[i] = (seq_t)(i + 1);

    /* apply P(k) for k = 2 .. n */
    for (size_t k = 2; k <= (size_t)n; ++k) {
        size_t start = k - 1;       /* zero‑based index of first element of a block */
        size_t len   = k;           /* block length */

        for (size_t s = start; s + len - 1 < M; s += len) {
            /* left‑rotate the block [s .. s+len-1] by one position */
            seq_t first = arr[s];
            for (size_t i = 0; i < len - 1; ++i)
                arr[s + i] = arr[s + i + 1];
            arr[s + len - 1] = first;
        }
    }

    seq_t result = arr[n - 1];     /* a(n) is at zero‑based index n-1 */
    free(arr);
    return result;
}
