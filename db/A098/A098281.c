/*
 * A098281  Back-to-front insertion-permutation sequence.
 * https://oeis.org/A098281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 119, u16 n <= 119, u32 n <= 119, u64 n <= 119,
 *            u128 n <= 119, big any n
 * Helpers:   gen_perm, get_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A098281 – “Back‑to‑front insertion‑permutation sequence”.
   a(n) is the n‑th decimal digit of the string obtained by concatenating
   all permutations of {1,…,k} (for k = 1,2,3,…), where the permutations are
   listed in the order produced by the back‑to‑front insertion algorithm
   described in the OEIS entry. */
#include <stdint.h>
#include "../utils/gen_perm.h"
#include "../utils/get_digit.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* minimal guard, offset is 1 */

    unsigned __int128 fact = 1;
    unsigned __int128 cum  = 0;     /* total digits from lengths < k */
    int k = 0;

    /* find minimal k with cum + k*k! >= n */
    for (;;) {
        ++k;
        fact *= (unsigned __int128)k;           /* k!   */
        unsigned __int128 group = (unsigned __int128)k * fact;  /* k*k! */
        if (cum + group >= n)
            break;
        cum += group;
    }

    /* offset within the k‑digit block (0‑based) */
    unsigned __int128 offset = (unsigned __int128)(n - 1) - cum;
    uint64_t permIndex = (uint64_t)(offset / (unsigned long long)k);
    uint64_t digitIndex = (uint64_t)(offset % (unsigned long long)k);

    int perm[20];
    gen_perm(k, permIndex, perm);
    return get_digit(perm, k, (int)digitIndex);
}
